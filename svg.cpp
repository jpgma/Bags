
//////////////////////
/// XML

#define XML_TAG 1
#define XML_CONTENT 2
#define XML_MAX_ATTRIBS 256
#define SVG_MAX_ATTRIBS (XML_MAX_ATTRIBS/2)

internal void
XMLParseContent (char *content, 
                 void (*content_callback)(void *parse_data, const char *s),
                 void *parse_data)
{
    // comendo espacos em branco
    while(*content && IS_SPACE_CHAR(*content)) content++;
    if(!*content) return;

    if(content_callback)
        (*content_callback)(parse_data, content);
}

internal void
XMLParseElement (char *element, 
                 void (*start_element_callback)(void *parse_data, const char *el, const char **attr),
                 void (*end_element_callback)(void *parse_data, const char *el),
                 void *parse_data)
{
    const char *attr[XML_MAX_ATTRIBS];
    s32 nattr = 0;
    char *name;
    s32 start = 0;
    s32 end = 0;
    char quote;

    // pular espacos brancos dps do '<'
    while (*element && IS_SPACE_CHAR(*element)) element++;

    // checar se e end tag
    if (*element == '/') 
    {
        element++;
        end = 1;
    } 
    else 
    {
        start = 1;
    }

    // pular comments e instrucoes de preproc.
    if (!*element || *element == '?' || *element == '!') return;

    // pegando nome da tag
    name = element;
    while(*element && !IS_SPACE_CHAR(*element)) element++;
    if(*element) *element++ = '\0';

    // pegando attribs
    while (!end && *element && nattr < XML_MAX_ATTRIBS-3) 
    {
        char *name = 0;
        char *value = 0;

        // pular espacos brancos antes do nome do attrib
        while (*element && IS_SPACE_CHAR(*element)) element++;
        if (!*element) break;
        if (*element == '/') 
        {
            end = 1;
            break;
        }
        name = element;
        // achar fim do nome do attrib
        while (*element && !IS_SPACE_CHAR(*element) && *element != '=') element++;
        if (*element) *element++ = '\0';
        // pular ate o comeco do valor
        while (*element && *element != '\"' && *element != '\'') element++;
        if (!*element) break;
        quote = *element;
        element++;
        // pega a quote, acha final do valor por ela
        value = element;
        while(*element && *element != quote) element++;
        if(*element) *element++ = '\0';

        // guardando apenas valores bem formados
        if(name && value)
        {
            attr[nattr++] = name;
            attr[nattr++] = value;
        }
    }

    // marcando final da lista de attribs
    attr[nattr++] = 0;
    attr[nattr++] = 0;

    // chamando callbacks
    if (start && start_element_callback)
        (*start_element_callback)(parse_data, name, attr);
    if (end && end_element_callback)
        (*end_element_callback)(parse_data, name);
}

internal void
XMLParse (char *xml_text,
          void (*start_element_callback)(void *parse_data, const char *el, const char **attr),
          void (*end_element_callback)(void *parse_data, const char *el), 
          void (*content_callback)(void *parse_data, const char *s),
          void *parse_data)
{
    char *c = xml_text;
    char *pos = c;
    u32 state = XML_CONTENT;
    while(*c)
    {
        if((*c == '<') && (state == XML_CONTENT))
        { // comecando tag
            *c++ = '\0';
            XMLParseContent(pos, content_callback, parse_data);
            pos = c;
            state = XML_TAG;
        }
        else if((*c == '>') && (state == XML_TAG))
        { // comecando content ou tag
            *c++ = '\0';
            XMLParseElement(pos, start_element_callback, end_element_callback, parse_data);
            pos = c;
            state = XML_CONTENT;
        }
        else
            c++;
    }
}

//////////////////////
/// SVG

struct SVGParseData
{
    VectorImage image;

    VIPathGroup *current_group;

    b32 in_path, in_defs;

    r32 dpi, font_size;
    u32 img_width, img_height;
    r32 view_min_x, view_min_y;
    r32 view_width, view_height;
};

global u32 debug_depth = 0;

enum SVGUnits
{
    SVG_UNITS_NONE,
    SVG_UNITS_PX,
    SVG_UNITS_PT,
    SVG_UNITS_PC,
    SVG_UNITS_MM,
    SVG_UNITS_CM,
    SVG_UNITS_IN,
    SVG_UNITS_EM,
    SVG_UNITS_EX,
    SVG_UNITS_PERCENT
};

internal u32
SVGGetUnits (char *units)
{
    u32 res = SVG_UNITS_NONE;
    
    if(units[0]=='p' && units[1]=='x') res = SVG_UNITS_PX;
    else if(units[0]=='p' && units[1]=='t') res = SVG_UNITS_PT;
    else if(units[0]=='p' && units[1]=='c') res = SVG_UNITS_PC;
    else if(units[0]=='m' && units[1]=='m') res = SVG_UNITS_MM;
    else if(units[0]=='c' && units[1]=='m') res = SVG_UNITS_CM;
    else if(units[0]=='i' && units[1]=='n') res = SVG_UNITS_IN;
    else if(units[0]=='e' && units[1]=='m') res = SVG_UNITS_EM;
    else if(units[0]=='e' && units[1]=='x') res = SVG_UNITS_EX;
    else if(units[0]=='%') res = SVG_UNITS_PERCENT;

    return res;
}

internal u32
SVGUnitsToPixels (SVGParseData *svgpd, r32 origin, r32 coordinate, r32 length, u32 units)
{
    u32 res = 0;

    switch(units)
    {
        default: // fall through
        case SVG_UNITS_NONE: // fall through
        case SVG_UNITS_PX:       res = coordinate;                            break;
        case NSVG_UNITS_PT:      res = coordinate / 72.0f * svgpd->dpi;       break;
        case NSVG_UNITS_PC:      res = coordinate / 6.0f * svgpd->dpi;        break;
        case NSVG_UNITS_MM:      res = coordinate / 25.4f * svgpd->dpi;       break;
        case NSVG_UNITS_CM:      res = coordinate / 2.54f * svgpd->dpi;       break;
        case NSVG_UNITS_IN:      res = coordinate * svgpd->dpi;               break;
        case NSVG_UNITS_EM:      res = coordinate * svgpd->font_size;         break;
        case NSVG_UNITS_EX:      res = coordinate * svgpd->font_size * 0.52f; break; // x-height of Helvetica.
        case NSVG_UNITS_PERCENT: res = origin + coordinate / 100.0f * length; break;
    }

    return res;
}

internal void
SVGStartElement (void *parse_data, const char *el, const char **attr)
{
    SVGParseData *svgpd = (SVGParseData *)parse_data;
    {
        // for (s32 i = 0; i < debug_depth; ++i) printf("\t");
        // debug_depth++;
        // printf("%s", el);
        // const char **a = attr;
        // while(a[0] && a[1])
        // {
        //     printf(" %s=\"%s\"", a[0],a[1]);
        //     a+=2;
        // }
        // printf("\n");
    }

    if(svgpd->in_defs)
    {
        if(AreEqual("linearGradient",el))
        {

        }
        else if(AreEqual("radialGradient",el))
        {

        }
        else if(AreEqual("stop",el))
        {
            
        }
        return;
    }

    if(AreEqual("g",el))
    {
        VIPathGroup *group = svgpd->current_group;
        if(group)
        {
            group->next = (VIPathGroup*)PlatformAllocateMemory(sizeof(VIPathGroup));
            group = group->next;
            svgpd->current_group = group;
        }
        else
        {
            svgpd->current_group = (VIPathGroup*)PlatformAllocateMemory(sizeof(VIPathGroup));
            group = svgpd->current_group;
            svgpd->image.first_group = group;
        }

        while(*attr)
        {
            if(AreEqual(attr[0],"id"))
            {
                u32 i = 0;
                const char *id = attr[1];
                while((i<16) && (id[i]!=0)) group->id[i] = id[i++];
                group->id[i] = 0;
            }

            attr+=2;
        }
    }
    else if(AreEqual("path",el))
    {
        if(svgpd->in_path) return; // support nested paths?
        svgpd->in_path = true;

        VIPathGroup *group = svgpd->current_group;
        if(!group)
        {
            group = (VIPathGroup*)PlatformAllocateMemory(sizeof(VIPathGroup));
            svgpd->current_group = group;
            svgpd->image.first_group = group;
        }

        VIPath *path = group->first_path;
        if(path)
        {
            while(path->next) 
                path = path->next;
            path->next = (VIPath*)PlatformAllocateMemory(sizeof(VIPath));
            path = path->next;
        }
        else
        {
            group->first_path = (VIPath*)PlatformAllocateMemory(sizeof(VIPath));
            path = group->first_path;
        }

        while(*attr)
        {
            if(AreEqual(attr[0],"id"))
            {
                u32 i = 0;
                const char *id = attr[1];
                while((i<16) && (id[i]!=0)) path->id[i] = id[i++];
                path->id[i] = 0;
            }
            attr+=2;
        }
    }
    else if(AreEqual("rect",el))
    {

    }
    else if(AreEqual("circle",el))
    {

    }
    else if(AreEqual("ellipse",el))
    {

    }
    else if(AreEqual("line",el))
    {

    }
    else if(AreEqual("polyline",el))
    {

    }
    else if(AreEqual("polygon",el))
    {

    }
    else if(AreEqual("linearGradient",el))
    {

    }
    else if(AreEqual("radialGradient",el))
    {

    }
    else if(AreEqual("stop",el))
    {

    }
    else if(AreEqual("defs",el))
    {
        svgpd->in_defs = true;
    }
    else if(AreEqual("svg",el))
    {
        while(*attr)
        {
            if(AreEqual("width",attr[0]))
            {
                r32 width = 0;
                char units[3] = {};
                sscanf(attr[1], "%f%s", &width,units);
                svgpd->img_width = SVGUnitsToPixels(svgpd, 0.0f, width, 0.0f, SVGGetUnits(units));
            }
            else if(AreEqual("height",attr[0]))
            {
                r32 height = 0;
                char units[3] = {};
                sscanf(attr[1], "%f%s", &height,units);
                svgpd->img_height = SVGUnitsToPixels(svgpd, 0.0f, height, 0.0f, SVGGetUnits(units));
            }
            else if(AreEqual("viewBox",attr[0]))
            {
                sscanf(attr[1], "%f%*[%%, \t]%f%*[%%, \t]%f%*[%%, \t]%f", 
                       &svgpd->view_min_x, &svgpd->view_min_y, &svgpd->view_width, &svgpd->view_height);
            }
            attr+=2;
        }

        
    }
}

internal void 
SVGEndElement (void *parse_data, const char *el)
{
    SVGParseData *svgpd = (SVGParseData *)parse_data;
    // {
    //     debug_depth--;
    //     for (s32 i = 0; i < debug_depth; ++i) printf("\t");
    //     printf("%s\n", el);
    // }

    if(AreEqual("g",el))
    {

    }
    else if(AreEqual("path",el))
    {
        svgpd->in_path = false;
    }
    else if(AreEqual("defs",el))
    {
        svgpd->in_defs = false;
    }
}

internal void
SVGParseContent (void *parse_data, const char *s)
{
    return;
}

internal void
SVGFreeParseData (SVGParseData *svgpd)
{
    if(svgpd)
    {

    }
}

internal VectorImage
VectorImageFromSVG (const char *filename, r32 dpi)
{
    VectorImage res = {};

    RawFileContents svg = PlatformGetRawFileContents(filename, 0);
    char *svg_text = (char*)svg.contents;
    svg_text[svg.size-1] = 0;

    if(svg_text)
    {
        SVGParseData svgpd = {};
        svgpd.dpi = dpi;

        XMLParse(svg_text, SVGStartElement, SVGEndElement, SVGParseContent, (void*)&svgpd);
        // Scale to viewBox
        // nsvg__scaleToViewbox(p, units);
        PlatformDeallocateMemory(svg.contents,svg.size);

        res = svgpd.image;
        res.width = svgpd.img_width;
        res.height = svgpd.img_height;
    }
    else
    {
        // log
    }

    return res;
}