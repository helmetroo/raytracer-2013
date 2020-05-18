/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents floating-point (32bpc) color.
 */

#ifndef COLOR_H
#define	COLOR_H

typedef struct _Color {
    float red;
    float green;
    float blue;
    float alpha;
} Color;

inline void initColor(Color* color, float red, float green, float blue, float alpha)
{
    color->red = red;
    color->green = green;
    color->blue = blue;
    color->alpha = alpha;
}

inline void copyColor(Color* src, Color* dest)
{
  dest->red = src->red;
  dest->green = src->green;
  dest->blue = src->blue;
  dest->alpha = src->alpha;
}

inline Color addColor(Color* first, Color* second)
{
  Color sumColor;
  initColor(&sumColor, first->red+second->red,
                        first->green+second->green,
                        first->blue+second->blue,
                        first->alpha+second->alpha);

  return sumColor;
}

inline Color multColor(Color* first, Color* second)
{
  Color sumColor;
  initColor(&sumColor, first->red*second->red,
                        first->green*second->green,
                        first->blue*second->blue,
                        first->alpha*second->alpha);

  return sumColor;
}

inline Color scalarMultColor(float scalar, Color* first)
{
  Color multColor;
  initColor(&multColor, 
	    scalar*first->red,
	    scalar*first->green,
	    scalar*first->blue,
	    scalar*first->alpha);

  return multColor;
}


#endif	/* COLOR_H */

