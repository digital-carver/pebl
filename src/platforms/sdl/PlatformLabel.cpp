//* -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*- */
/////////////////////////////////////////////////////////////////////////////////
//    Name:       src/platforms/sdl/PlatformLabel.cpp
//    Purpose:    Contains SDL-specific visual representation of a word
//    Author:     Shane T. Mueller, Ph.D.
//    Copyright:  (c) 2004 Shane T. Mueller <smueller@umich.edu>
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
#include "PlatformLabel.h"
#include "../../objects/PLabel.h"
#include "PlatformFont.h"
#include "../../utility/rc_ptrs.h"
#include "../../utility/PError.h"

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <stdio.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::list;
using std::ostream;



PlatformLabel::PlatformLabel(const std::string & text, counted_ptr<PlatformFont> font):
    PlatformWidget(),
    PLabel(text),
    mFont(font)
{
    Draw();
}


PlatformLabel::PlatformLabel(PlatformLabel & label):
    PlatformWidget(),
    PLabel(label.GetText())
{

    mFont = label.GetFont();
    Draw();
}


///Standard Destructor
PlatformLabel::~PlatformLabel()
{
}


// Inheritable function that is called by friend method << operator of PComplexData
ostream & PlatformLabel::SendToStream(ostream& out) const
{
    out << "<SDL PlatformLabel: " << mText << " in " << *mFont << ">" <<flush;
    return out;
};

 


/// This method should be called when the font is initialized or the text is changed.
/// It will make the mSurface pointer hold the appropriate image.
bool  PlatformLabel::RenderText()
{

    //free the memory if it is currently pointing at something.
    if(mSurface)  SDL_FreeSurface(mSurface);

    //Re-render the text using the associated font.
    mSurface = mFont->RenderText(mText.c_str());
    
    //If mSurface is null, then rendering failed.
    if( mSurface)
        {
            mWidth  = mSurface->w; 
            mHeight = mSurface->h;

            return true;
        }


    else
        return false;
}


void PlatformLabel::SetFont(counted_ptr<PlatformFont> font)
{
    //Chain up to parent method
    mFont = font;
    mTextChanged =true;
    Draw();
    //Re-render the text onto mSurface
    //if(!RenderText()) cerr << "Unable to render text.\n";

}


void PlatformLabel::SetText(const std::string & text)
{
    //Chain up to parent method.
    PTextObject::SetText(text);
    mTextChanged =true;
    Draw();

    //Re-render the text onto mSurface
    //if(!RenderText()) cerr << "Unable to render text.\n";

}


bool PlatformLabel::Draw()
{

    if(mTextChanged)
        {
            RenderText();
            //Reposition.  This just recalculates so things are centered
            //correctly; labels are positioned based on their center.
            SetPosition(mX, mY);
            mTextChanged = false;
        }
    return  PlatformWidget::Draw();

}
