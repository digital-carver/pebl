//* -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*- */
/////////////////////////////////////////////////////////////////////////////////
//    Name:       libs/PEBLStream.h
//    Purpose:    Stream-based Function Library for PEBL
//    Author:     Shane T. Mueller, Ph.D.
//    Copyright:  (c) 2003-2005 Shane T. Mueller <smueller@obereed.net>
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
#ifndef __PEBLSTREAM_H__
#define __PEBLSTREAM_H__

class Variant;


namespace PEBLStream
{
    ///This prints the argument with a newline character
    Variant Print(Variant v);

    ///This prints the argument without a linebreak at the end
    Variant Print_(Variant v);

    ///This operates like print but without the printing.  
    ///Useless right now, but could be used for formatting output,
    ///changing numbers to text, etc.
    Variant Format(Variant v);

    ///This opens a filestream for reading.
    Variant FileOpenRead(Variant v);
    
    ///This opens a filestream for writing
    Variant FileOpenWrite(Variant v);

    Variant FileOpenAppend(Variant v);

    ///This Closes a filestream
    Variant FileClose(Variant v);

    ///This prints contents of variant to file, followed by a newline
    Variant FilePrint(Variant v);

    ///This prints contents of variant to file, without appending a newline
    Variant FilePrint_(Variant v);
    
    ///Reads a character from a file
    Variant FileReadCharacter(Variant v);

    ///Reads everything up until the next space/newline, dropping all
    ///initial spaces/newlines
    Variant FileReadWord(Variant v);

    ///Reads until the next line
    Variant FileReadLine(Variant v);

    ///Places all words in file into a list
    Variant FileReadList(Variant v);
    
    ///Makes a list of lists, with each row contained in a list.
    Variant FileReadTable(Variant v);

    ///This will read all of the text in a file into a single variant.
    Variant FileReadText(Variant v);
    
    Variant EndOfLine(Variant v);
    Variant EndOfFile(Variant v);


    // The following control TCP/IP connections and communications.

    Variant ConnectToIP(Variant v);
    Variant ConnectToHost(Variant v);
    Variant WaitForNetworkConnection(Variant v);
    Variant CloseNetworkConnection(Variant v);

    Variant SendData(Variant v);
    Variant GetData(Variant v);


    //This writes a 'screenshot' to a png file.
    Variant WritePNG(Variant v);


}
#endif
