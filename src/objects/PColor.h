//* -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*- */
/////////////////////////////////////////////////////////////////////////////////
//    Name:       src/objects/PColor.h
//    Purpose:    Lightweight class representing color
//    Author:     Shane T. Mueller, Ph.D.
//    Copyright:  (c) 2003 Shane T. Mueller <smueller@umich.edu>
//    License:    GPL 2
//
//
//
//     This file is part of the PEBL project.
//
//    PEBL is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    PEBL is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with PEBL; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////
#ifndef __PCOLOR_H__
#define __PCOLOR_H__

#include "PObject.h"
#include <iostream>
#include <string>

///  This class represents color. Currently, only RGBA color is  accessible.
///  Access through other color spaces could be added.
class PColor: public PObject
{
public:
    PColor();                                             ///Constructor
    PColor(unsigned int color){SetColorByRGBA(color);}    ///Constructor
    PColor(int red, int green, int blue, int alpha);      ///Constructor
    PColor(const std::string & colorname);                     ///Constructor using RGBColorNames

    PColor(const PColor & pcolor);                        ///Copy constructor
    virtual ~PColor(){}                                   ///Standard Destructor
  

    //These will return a single color
    int GetRed  () const { return mRed;};
    int GetGreen() const { return mGreen;};
    int GetBlue () const { return mBlue;};
    int GetAlpha() const { return mAlpha;};
 
    //These will set a single color
    void SetRed  (int color){mRed   = To8BitColor(color);}
    void SetGreen(int color){mGreen = To8BitColor(color);}
    void SetBlue (int color){mBlue  = To8BitColor(color);}
    void SetAlpha(int color){mAlpha = To8BitColor(color);}


    ///Sets color as a 32-bit unsigned int.
    void SetColorByRGBA(unsigned int color);

    ///Sets color as a 4 8-bit unsigned int.
    void SetColorByRGBA(int red, int green, int blue, int alpha);

    ///Sets color as by its name.
    void SetColorByName(const std::string & colorname);

    ///Gets 32-bit unsigned int color
    unsigned int GetColor() const;

protected:

    //Used by overloaded << operator in PComplexData.
    virtual std::ostream & SendToStream(std::ostream& out) const;

private:

    //Private function that ensures colors are between 0 and 255.
    int To8BitColor(int color);

    ///Each of the colors is represented by an 8-bit array:
    unsigned mRed:   8;
    unsigned mGreen: 8;
    unsigned mBlue:  8;
    unsigned mAlpha: 8;
};


#endif
