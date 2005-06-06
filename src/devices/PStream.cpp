//* -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*- */
/////////////////////////////////////////////////////////////////////////////////
//    Name:       src/devices/PStream.cpp
//    Purpose:    Primary File Stream class
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
#include "PStream.h"
#include "../utility/PError.h"

#include <iostream>
#include <fstream>
#include <sstream> 
#include <list>
#include <string>


using std::ostream;
using std::fstream;
using std::flush;
using std::string;
using std::cout;
using namespace std;  //Ugh...need this for ios_base?

/// This is the standard pNode constructor
PStream::PStream(const char* filename, StreamDirection dir, StreamType type):
    mStreamFileName(strdup(filename)),
    mStreamDirection(dir),
    mStreamType(type)
{
    //Standard Constructor
          
    
    //Now, create the stream with proper flags and ready it for
    //input/output.

    switch (mStreamDirection)
        {
        case sdRead:
            mFileStream = new fstream(mStreamFileName,  ios_base::in);        
            break;

        case sdWrite:
            mFileStream = new fstream(mStreamFileName,  ios_base::out);
            break;

        case sdAppend:
            mFileStream = new fstream(mStreamFileName, ios_base::out | ios_base::app);
            break;
        default:

            //error
            std::cout << "Error occurred in PStream Constructor\n";
        }
    //Binary/ascii/unicode/etc could be done here as well.


            //If we didn't manage to encode anything, call an error
    if(!mFileStream)
        {
            PError::SignalFatalError("Error initiating file [" + string(mStreamFileName) + ".");
        }
}



/// This is the standard pNode constructor
void PStream::Open(const char* filename, StreamDirection dir, StreamType type)
{
  
    if(mStreamFileName)
        free(mStreamFileName);

    
    mStreamFileName  = strdup(filename);
    mStreamDirection = dir;
    mStreamType      = type;

    //Now, create the stream with proper flags and ready it for
    //input/output.


    if(mStreamDirection==sdRead)
        {
            mFileStream->open(mStreamFileName,  ios_base::in);        
        }
    else if (mStreamDirection==sdWrite)
        {
            mFileStream->open(mStreamFileName,  ios_base::out);        
            
        }
    else if (mStreamDirection == sdAppend)
        {
            mFileStream->open(mStreamFileName, ios_base::out | ios_base::app);
        }
    else
        {
            //error
            std::cout << "Error occurred in PStream::Open\n";
        }
    //Binary/ascii/unicode/etc could be done here as well.
    
   
    //If we didn't manage to encode anything, call an error
    if(!mFileStream)
        {
            PError::SignalFatalError("Error initiating file [" + string(mStreamFileName) + ".");
        }
    
}


///This method sends a single character to the stream
void PStream::WriteChar(const char character)
{
    if(mStreamDirection == sdWrite ||
       mStreamDirection == sdAppend)
        {
            (*mFileStream) << character << flush;
        }
    else
        {
           PError::SignalFatalError("Error in PStream::WriteChar--trying to write a Read stream: "+ string(mStreamFileName) + "."); 
        }

}


/// This method sends the number of bytes specified by the length 
/// parameter from the buffer to the stream.  This could be dangerous
/// if the user is allowed to specify these things themselves.
void PStream::WriteBuffer(const char* buffer, unsigned int length)
{

    if(mStreamDirection == sdWrite ||
       mStreamDirection == sdAppend)
        {
            
            char * tmp = strdup(buffer);
            //            cout << "Writing buffer to file: ["<< tmp << "]\n";
         //This is really dangerous.
               tmp[length]='\0';  //add an end-of-string character
            
            (* mFileStream) << tmp << flush;
            free(tmp);
            
        }
    else
        {
            PError::SignalFatalError("Error in PStream::WriteBuffer--trying to write a Read stream: "+ string(mStreamFileName) + ".");
        }
}


///This method just writes the char* string to the stream.
void PStream::WriteString(const char* buffer)
{
    if(mStreamDirection == sdWrite ||
       mStreamDirection == sdAppend)
        {
            
            char * tmp = strdup(buffer);          
            (* mFileStream) << tmp << flush;
            free(tmp);

        }
    else
        {   
            PError::SignalFatalError("Error in PStream::WriteString--trying to write a Read stream: "+ string(mStreamFileName) + ".");
        }
    
}


/// This reads a single character from the filestream 
/// and returns it.
char PStream::ReadChar()
{
    if(mStreamDirection == sdRead)
        {
            
            char tmp;
            mFileStream->get(tmp);
            return tmp;
                       
        }
    else
        {

            PError::SignalFatalError("Error in PStream::ReadChar--trying to Read a Write stream\n");
            return 0;
        }
}



/// This reads up until the next separator token (or eof character)
std::string PStream::ReadToken(const char separator)
{
    if(mStreamDirection == sdRead)
        {
            
            std::string tmpstring;  //Create a stream to use to capture input.      
            char tmp;
            while(!mFileStream->eof() )
                {
                    tmp = mFileStream->peek();
                    if (Eol())
                        {
                            //mFileStream->get(tmp);  
                            break;
                        }
                    
                    else  if(tmp == separator )
                        {
                            mFileStream->get(tmp);
                            break;
                        }
                    else
                        {
                            mFileStream->get(tmp);
                            tmpstring += tmp;   
                        }
                }
            return tmpstring;
        }
    else
        {
            PError::SignalFatalError("Error in PStream::ReadToken--trying to Read a Write stream\n");
            return 0;
        }
}


/// This reads up until the next eol/eof character, including the newline 
/// character at the end.
std::string PStream::ReadLine()
{
    if(mStreamDirection == sdRead)
        {
            std::string tmpstring;
            char tmp;
            mFileStream->get(tmp);


            while( !mFileStream->eof())
                {
                    tmpstring += tmp; 
                    if(tmp == 10)       //10 is the newline character         
                        break;          //Break out if the next character is eol
                    
                    mFileStream->get(tmp);
                }


            return tmpstring;
        }
          
    else
        {
            PError::SignalFatalError("Error in PStream::ReadLine--trying to Read a Write stream\n");
            return 0;
        }
}
    


// This reads a line from a stream, stripping the newline character from the end.
//
std::string PStream::ReadLineClean()
{
    if(mStreamDirection == sdRead)
        {
            std::string tmpstring;
            char tmp;
            mFileStream->get(tmp);


            while( !mFileStream->eof())
                {

                    if(tmp == 10)       //10 is the newline character         
                        break;          //Break out if the next character is eol

                    tmpstring += tmp;                     
                    mFileStream->get(tmp);
                }


            return tmpstring;
        }
          
    else
        {
            PError::SignalFatalError("Error in PStream::ReadLine--trying to Read a Write stream\n");
            return 0;
        }
}
    



bool PStream::Eol()
{
    if(mFileStream->peek() == 10)  //LF
        return true;
    
    if(mFileStream->peek() == 13)  //CR (precedes an LF in DOS
        return true;
    
    if(mFileStream->eof())   //EOF --treat as an EOL.)
        return true;
    
    return false;
}

bool PStream::Eof()
{
    return mFileStream->eof();
}

//This closes an open stream.
bool PStream::Close()
{
    mFileStream->close();
    return true;
}

/// This is the standard pNode destructor
PStream::~PStream()
{
    // Standard Destructor
    delete mStreamFileName;
}


ostream & PStream::SendToStream(ostream & out) const
{
    out << "<Generic File Stream>" << flush;
    return out;
}

