//* -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*- */
////////////////////////////////////////////////////////////////////////////////
//    Name:       libs/PEBLStream.cpp
//    Purpose:    Built-in stream functions for PEBL
//    Author:     Shane T. Mueller, Ph.D.
//    Copyright:  (c) 2003-2011 Shane T. Mueller <smueller@obereed.net>
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
#include "PEBLStream.h"
#include "../base/Variant.h"
#include "../base/PComplexData.h"
#include "../base/PList.h"
#include "../base/Evaluator.h"

#include "../devices/PStream.h"
#include "../utility/PError.h"
#include "../utility/PEBLPath.h"
#include "../utility/PEBLUtility.h"
#include "../devices/PParallelPort.h"


#include <list>
#include <iostream>
#include <fstream>

//This is for pport stuff used for testing.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if defined(PEBL_LINUX)
#include <sys/io.h>
#endif

#define BASE 0x378

const static int BSIZE =4096;

using std::cout;
using std::flush;
using std::cerr;
using std::endl;
using std::ios_base;


Variant PEBLStream::Print(Variant v)
{

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First();
    //No type assertion is needed, because everything should work.
    cout << v1 << endl;

    return v1;
}


Variant PEBLStream::Print_(Variant v)
{

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First();
    //No type assertion is needed, because everything should work.
    cout << v1 << flush;

    return v1;
}

Variant PEBLStream::Format(Variant v)
{  

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First();
    PError::SignalFatalError("Function [Format()] not implemented.");
    return v1;
}


///This opens a filestream for reading.
Variant PEBLStream::FileOpenRead(Variant v)
{
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in function FileOpenRead(<string>)]: ");  
    
    ///v1 contains the name of a file to open.
    counted_ptr<PEBLObjectBase> mystream = counted_ptr<PEBLObjectBase>(new PStream(v1,sdRead, stASCII));
    PComplexData * pcd = new PComplexData(mystream);
    Variant tmp = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp;
}

///This opens a filestream for writing, replacing current file
Variant PEBLStream::FileOpenWrite(Variant v)
{


    // should be able to create the directory if it doesn't exist.
    //    char sysText[256];
    //    sprintf( systext, "mkdir -p %s", path ); // path is a char * to the string representing the directory you want to create
    //    system( systext );
 

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in function FileOpenWrite(<string>)]: ");  
 
    ///v1 contains the name of a file to open.      
    counted_ptr<PEBLObjectBase> mystream = counted_ptr<PEBLObjectBase>(new PStream(v1,sdWrite, stASCII));
    PComplexData * pcd = new PComplexData(mystream);
    Variant tmp = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp;

}

///This opens a filestream for writing, appending to end of file.
Variant PEBLStream::FileOpenAppend(Variant v)
{
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in function FileOpenAppend(<string>)]: ");  

    ///v1 contains the name of a file to open.    
    counted_ptr<PEBLObjectBase> mystream = counted_ptr<PEBLObjectBase>(new PStream(v1,sdAppend, stASCII));
    PComplexData * pcd = new PComplexData(mystream);
    Variant tmp = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp;


}

///This Closes a filestream
Variant PEBLStream::FileClose(Variant v)
{
    //v[1] should have the file stream to close
    PList * plist = v.GetComplexData()->GetList();
    
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_FILESTREAM, "Argument error in function [FileClose(<file-stream>)]: ");  


    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PStream * mystream = dynamic_cast<PStream*>(tmp2.get());
    return Variant(mystream->Close());
}


Variant PEBLStream::FilePrint(Variant v)
{
    //v[1] should have the file stream 
    //v[2] should have the variant to print
    PList * plist = v.GetComplexData()->GetList();

    Variant v1 = plist->First(); plist->PopFront();


    PError::AssertType(v1, PEAT_FILESTREAM, "Argument error in first parameter of function [FilePrint(<file-stream>, <text>)]: ");  

    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PStream * mystream = dynamic_cast<PStream*>(tmp2.get());


    Variant v2 = plist->First(); plist->PopFront();
    std::string s = v2.GetString();
        
    mystream->WriteString(s + "\n");
    return (v2+Variant("\n"));
}  

Variant PEBLStream::FilePrint_(Variant v)
{
    //v[1] should have the file stream 
    //v[2] should have the variant to print
    PList * plist = v.GetComplexData()->GetList();

    Variant v1 = plist->First(); plist->PopFront();

    PError::AssertType(v1, PEAT_FILESTREAM, "Argument error in first parameter of function [FilePrint_(<file-stream>, <text>)]: ");  

    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PStream * mystream = dynamic_cast<PStream*>(tmp2.get());

    Variant v2 = plist->First(); plist->PopFront();
    mystream->WriteString(v2);
    return v2;
}

Variant PEBLStream::FileReadCharacter(Variant v)
{
    //v[1] should have the file stream 
    PList * plist = v.GetComplexData()->GetList();

    Variant v1 = plist->First(); plist->PopFront();

    PError::AssertType(v1, PEAT_FILESTREAM, "Argument error in function [FileReadCharacter(<file-stream>)]: ");   
    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();

    PStream * mystream = dynamic_cast<PStream*>(tmp2.get());
    char c = (mystream->ReadChar());
    return Variant((char)c);
}

Variant PEBLStream::FileReadWord(Variant v)
{
    //v[1] should have the file stream 
    PList * plist = v.GetComplexData()->GetList();

    Variant v1 = plist->First(); plist->PopFront();

    PError::AssertType(v1, PEAT_FILESTREAM, "Argument error in function [FileReadWord(<file-stream>)]: ");   

    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PStream * mystream = dynamic_cast<PStream*>(tmp2.get());

    return Variant(mystream->ReadToken(' '));
}


//This takes a filestream and returns the next line.
Variant PEBLStream::FileReadLine(Variant v)
{
    PList * plist = v.GetComplexData()->GetList();
    //v[1] should have the file stream    
    Variant v1 = plist->First(); plist->PopFront();

    PError::AssertType(v1, PEAT_FILESTREAM, "Argument error in function [FileReadLine(<file-stream>)]: ");   
    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PStream * mystream = dynamic_cast<PStream*>(tmp2.get());

    return Variant(mystream->ReadLineClean());
}




///This function takes a filename as an argument.  It
///Opens the file, and reads in each line of the file into a string,
///which it places in a list that it returns.  It closes the file afterward.
Variant PEBLStream::FileReadList(Variant v)
{

   //v[1] should have the filename
    PList * plist = v.GetComplexData()->GetList();

    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in function [FileReadList(<filename>)]:  ");    
 
    //Search through the paths for the file, because it may be a 'special' file.
    string filename = Evaluator::gPath.FindFile(v1.GetString());
    
    if(filename == "")
        PError::SignalFatalError(string("Unable to find file [")  + v1.GetString() + string("]."));

    //It must be good, so open it.
    PStream *  mystream = new PStream(filename,sdRead, stASCII);
   


    PList * returnlist = new PList();
    std::string  tmpstring;
    while(!mystream->Eof())
        {
            tmpstring = mystream->ReadLineClean();

            if(strcmp("",tmpstring.c_str()) &&       //Ignore blank lines
               strncmp("#",tmpstring.c_str(),1)      //Ignore lines starting with #
               )
                {
                    returnlist->PushBack(tmpstring);
                }
        }
    mystream->Close();
    delete mystream;

    counted_ptr<PEBLObjectBase>  tmp2 = counted_ptr<PEBLObjectBase>(returnlist);
    PComplexData * pcd = new PComplexData(tmp2);
    Variant tmp3 = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp3;
}
    

///Give this function a filename, and it will read it into a list 
///of lists; each line is a list, and the (optional) token-separated elements
///are the items of the list.
Variant PEBLStream::FileReadTable(Variant v)
{
    //v[1] should have the file stream 
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in first parameter of function [FileReadTable(<list>)]:  ");    
     
    //Search through the paths for the file, because it may be a 'special' file.
    string filename = Evaluator::gPath.FindFile(v1.GetString());
    
    if(filename == "")
        PError::SignalFatalError(string("Unable to find file [")  +v1.GetString() + string("]."));

    //It must be good, so open it.
    PStream *   myStream = new PStream(filename,sdRead, stASCII);
    
    char separator;
    //See if there is another parameter; if there is, it is the token separator.
    if(plist->IsEmpty())
        {
            separator = ' ';
        }
    else
        {
            PError::AssertType(plist->First(), PEAT_STRING, "Argument error in second parameter of function [FileReadTable(<list>)]:  ");    
            std::string tmp = plist->First(); plist->PopFront();
            separator = tmp[0];
        }

    //Make an outer list to put everything in.

    PList * returnlist = new PList();

    //Make an inner list
    Variant innerlist;

    std::string tmpstring;

    while(!myStream->Eof())
        {

            tmpstring = myStream->ReadLineClean();
            if((strcmp("",tmpstring.c_str()) == 0) ||      //Ignore blank lines
               (strncmp("#",tmpstring.c_str(),1)==0)      //Ignore lines starting with #
               )
                {

                    //This line is garbage, so read it and throw it away.
                
                }
            else
                {
                    innerlist =  PEBLUtility::Tokenize(tmpstring.c_str(),separator);
                    returnlist->PushBack(innerlist);
                }
        }
    myStream->Close();
    delete myStream;
    counted_ptr<PEBLObjectBase> tmp2 =  counted_ptr<PEBLObjectBase>(returnlist);
    PComplexData * pcd = new PComplexData(tmp2);
    Variant tmp3 = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp3;

}


///This function takes a filename as an argument.  It
///Opens the file, and reads in the entire file as a string,
///which it  returns, and closes the file afterward. It ignores any
///lines that begin with #, and converts '\n' to [10], which
///is later parsed as a carriage return.
Variant PEBLStream::FileReadText(Variant v)
{

   //v[1] should have the filename
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in function [FileReadText(<filename>)]:  ");    
 
    //Search through the paths for the file, because it may be a 'special' file.
    string filename = Evaluator::gPath.FindFile(v1.GetString());
    
    if(filename == "")
        PError::SignalFatalError(string("Unable to find file [")  + v1.GetString() + string("]."));

    //It must be good, so open it.
    PStream * mystream = new PStream(filename,sdRead, stASCII);
   

    Variant returnText = "";
    std::string tmpstring;
    while(!mystream->Eof())
        {
            tmpstring = mystream->ReadLine();

            if( strncmp("#",tmpstring.c_str(),1)      //Ignore lines starting with #
               )
                {
                    //This is wrong--it will add extra space where it is not needed.
                    returnText = returnText +  Variant(tmpstring);
                }
        }
    mystream->Close();
    delete mystream;

    return returnText;
}
    


///This detects if you are at the end of a line
Variant PEBLStream::EndOfLine(Variant v)
{
   //v[1] should have the file stream 
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();

    PError::AssertType(v1, PEAT_FILESTREAM, "Argument error in function [EndOfLine(<file-stream>)]: ");   
    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PStream * mystream = dynamic_cast<PStream*>(tmp2.get());
    return Variant(mystream->Eol());
}


///This detects if you are at the end of a file
Variant PEBLStream::EndOfFile(Variant v)
{
   //v[1] should have the file stream 
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();

    PError::AssertType(v1, PEAT_FILESTREAM, "Argument error in function [EndOfFile(<file-stream>)]: ");   
    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PStream * mystream = dynamic_cast<PStream*>(tmp2.get());
    return Variant(mystream->Eof());
}


    //Copies the contents of one file to another.
Variant PEBLStream::AppendFile(Variant v)
{
    //v[1] should have the filename
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in function [AppendFile(<file1>,<file2>)]:  ");    
 
    string filename1 = v1.GetString();

    
    Variant v2 = plist->First(); 
    PError::AssertType(v2, PEAT_STRING, "Argument error in function [AppendFile(<file1>,<file2>)]:  ");    
 
    //Search through the paths for the file, because it may be a 'special' file.
    string filename2 = Evaluator::gPath.FindFile(v2.GetString());
    
    if(filename2 == "")
        PError::SignalFatalError(string("Unable to find file [")  + v2.GetString() + string("] in AppendFile"));

    //open files in binary mode so we can do a direct copy.
    
    std::ifstream in(filename2.c_str(), ios_base::in|ios_base::binary);
    std::ofstream out(filename1.c_str(), ios_base::out|ios_base::app|ios_base::binary);


    if(!in.is_open())
        {
            PError::SignalWarning("Unable to open file ["+filename2+"]. No data copied.");
        }
    if(!out.is_open())
        {
            PError::SignalFatalError("Unable to open file ["+filename1+"]. Check permissions or determine whether directory exists.");
        }


    char buf[BSIZE];
    do {
        in.read(&buf[0],BSIZE);
        out.write(&buf[0],in.gcount());
    }while (in.gcount()>0);

    in.close();
    out.close();
    return Variant(1);
}






Variant PEBLStream::ConnectToIP(Variant v)
{
    //v[1] should have the IP Number as an integer
    //v[2] should have the port number

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_INTEGER, "Argument error in first parameter of function [ConnectToHost(<hostname>,<port>)]: ");

    Variant v2 = plist->First();
    PError::AssertType(v2, PEAT_INTEGER, "Argument error in second parameter of function [ConnectToHost(<hostname>,<port>)]: ");   

    
    counted_ptr<PEBLObjectBase> tmp2 = counted_ptr<PEBLObjectBase>(new PlatformNetwork());
    PlatformNetwork * mynet = dynamic_cast<PlatformNetwork*>(tmp2.get());

    mynet->Init();
    mynet->SetHostIP((long unsigned int)v1);
    mynet->SetPort((int)v2);
    mynet->Open();

    PComplexData * pcd = new PComplexData(tmp2);
    Variant tmp3 = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp3;
}

Variant PEBLStream::ConnectToHost(Variant v)
{

    //v[1] should have the host name
    //v[2] should have the port number
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in first parameter of function [ConnectToHost(<hostname>,<port>)]: ");

    Variant v2 = plist->First();
    PError::AssertType(v2, PEAT_INTEGER, "Argument error in second parameter of function [ConnectToHost(<hostname>,<port>)]: ");   
    
    
    counted_ptr<PEBLObjectBase> tmp2 = counted_ptr<PEBLObjectBase>(new PlatformNetwork());
    PlatformNetwork * mynet = dynamic_cast<PlatformNetwork*>(tmp2.get());

    mynet->Init();
    mynet->SetHostName(v1);
    mynet->SetPort((int)v2);


    mynet->Open();

    if(mynet->IsOpen())
        {
            

            PComplexData * pcd = new PComplexData(tmp2);
            Variant tmp3 = Variant(pcd);
            delete pcd;
            pcd=NULL;
            return tmp3;
            
        }else{

        return Variant(0);
    }
    

}





Variant PEBLStream::SetNetworkPort(Variant v)
{
    //v[1] should have the port number
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();

    PError::AssertType(v1, PEAT_INTEGER, "Argument error in first parameter of function [WaitForNetworkConnection(<port>)]: ");

    
    counted_ptr<PEBLObjectBase> tmp2 = counted_ptr<PEBLObjectBase>(new PlatformNetwork());
    PlatformNetwork * mynet = dynamic_cast<PlatformNetwork*>(tmp2.get());
    mynet->Init();
    mynet->SetHostName("");
    mynet->SetPort((int)v1);
    //    mynet->Accept();

    PComplexData * pcd = new PComplexData(tmp2);
    Variant tmp3 = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp3;
}



Variant PEBLStream::CheckForNetworkConnection(Variant v)
{
    //v[1] should have the network object which should have been opened but not yet
    //accepted a connection
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_NETWORKCONNECTION, "Argument error in first parameter of function [CheckFroNetworkConnection(<network>)]: ");   
    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PlatformNetwork * mynet = dynamic_cast<PlatformNetwork*>(tmp2.get());

    

    bool result ;    //


    mynet->Init();

    mynet->SetHostName("");


    //mynet->SetPort((int)v1);


    result = mynet->CheckForConnection();

    //    mynet->Accept();
    
    //PComplexData * pcd = new PComplexData(tmp2);
    //    Variant tmp3 = Variant(pcd);
    //    delete pcd;
    //    pcd=NULL;
    //    return tmp3;
    return Variant(result);
}



Variant PEBLStream::WaitForNetworkConnection(Variant v)
{

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();

    PError::AssertType(v1, PEAT_INTEGER, "Argument error in first parameter of function [WaitForNetworkConnection(<port>)]: ");

    
    counted_ptr<PEBLObjectBase> tmp2 = counted_ptr<PEBLObjectBase>(new PlatformNetwork());
    PlatformNetwork * mynet = dynamic_cast<PlatformNetwork*>(tmp2.get());
    mynet->Init();
    mynet->SetHostName("");
    mynet->SetPort((int)v1);
    mynet->Accept();

    PComplexData * pcd = new PComplexData(tmp2);
    Variant tmp3 = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp3;

}

Variant PEBLStream::CloseNetworkConnection(Variant v)
{
    //v[1] should have the network connection
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_NETWORKCONNECTION, "Argument error in function [CloseNetworkConnection(<network>)]: ");   

    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PlatformNetwork * mynet = dynamic_cast<PlatformNetwork*>(tmp2.get());
    mynet->Close();

    return Variant(1);
    
}

Variant PEBLStream::SendData(Variant v)
{
    //v[1] should have the Network connection object
    //v[2] should have the data
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_NETWORKCONNECTION, "Argument error in first parameter of function [SendData(<network>,<data>)]: ");   
    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PlatformNetwork * mynet = dynamic_cast<PlatformNetwork*>(tmp2.get());

    Variant v2 = plist->First();
    PError::AssertType(v2, PEAT_STRING, "Argument error in second parameter of function [SendData(<network>,<data>)]: ");   
    mynet->SendString(v2);
    return Variant(1);
}

Variant PEBLStream::GetData(Variant v)
{
    //v[1] should have the Network connection object
    //v[2] should have the max length of the data we are looking for.

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_NETWORKCONNECTION, "Argument error in first parameter of function [GetData(<network>,<size>)]: ");   
    counted_ptr<PEBLObjectBase> tmp2 = (v1.GetComplexData())->GetObject();
    PlatformNetwork * mynet = dynamic_cast<PlatformNetwork*>(tmp2.get());

    Variant v2 = plist->First();
    PError::AssertType(v2, PEAT_INTEGER, "Argument error in second parameter of function [GetData(<network>,<size>)]: ");   

    Variant ret = mynet->Receive(v2);
    return ret;

}




Variant PEBLStream::WritePNG(Variant v)
{
    //v[1] should have an object/window name.
    //v[2] should have a filename

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in first parameter of function [WritePNG(<filename>,<object>)]: ");   


    Variant v2 = plist->First();
    PError::AssertType(v2, PEAT_WIDGET, "Argument error in second parameter of function [WritePNG(<filename>,<object>)]: ");   
    counted_ptr<PEBLObjectBase> tmp2 = (v2.GetComplexData())->GetObject();
    PlatformWidget * myobj = dynamic_cast<PlatformWidget*>(tmp2.get());


    int result = SDLUtility::WritePNG(v1,myobj);

    if(result==0)
        {
            PError::SignalFatalError("Failed to write png file " + v2);
        }

    return Variant(1);
}



Variant PEBLStream::OpenPPort(Variant v)
{
    //v[1] should have a text string indicating
    //lpt1/lpt2/lpt3 (which may not really map onto
    //those ports for any individual system.


    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_STRING, "Argument error in first parameter of function [OpenPPort(<port>)]: ");

    unsigned char data;       /* value of the data register */
    unsigned char control;    /* value of the control register */
    
#if 0
    if(0)
        {
            //hardcode some parport stuff here for testing.
            if (iopl(3))
                printf("Could't get the port addresses\n");
            


            /* enable data register output by bringing bit 5 of control */
            /* register low                                             */
            
            control = inb(BASE + 2);
            cout << "control byte:" << (int)control << endl;
            outb(control & ~0x20, BASE + 2);
            
            /* write the value */
            outb((unsigned char) 255, BASE);
            
            /* Wait a few secs, so we can see it.*/
            struct timespec a,b;
            a.tv_sec  = 5;   
            a.tv_nsec = 100000; //1 ms
            int retval = nanosleep(&a,&b);
            
            /* disable data register output by bringing bit 5 of control */
            /* register high                                             */
            
            control = inb(BASE + 2);
            cout << "control byte:" << (int)control << endl;
            outb(control | 0x20, BASE + 2);
            
            
            /* read and print the value */
            printf("Port 0x%x reads 0x%x\n", BASE, inb(BASE));
        }
#endif

    counted_ptr<PEBLObjectBase> tmp2 = counted_ptr<PEBLObjectBase>(new PParallelPort());
    PParallelPort * pport = dynamic_cast<PParallelPort*>(tmp2.get());
    
    
    pport->SetPort(v1);
    pport->Init();
            

    PComplexData * pcd = new PComplexData(tmp2);
    Variant tmp3 = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp3;

}


//This sets the pport state on the data bits
Variant PEBLStream::SetPPortState(Variant v)
{
   //v[1] should have an pport 
   //v[2] should have a list of bits 0/1

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_PARALLELPORT, "Argument error in first parameter of function [SetPPortState(<pport>,<integer>)]: ");   

    counted_ptr<PEBLObjectBase> tmp1 = (v1.GetComplexData())->GetObject();
    PParallelPort * mypport = dynamic_cast<PParallelPort*>(tmp1.get());
    

    Variant v2 = plist->First();
    PError::AssertType(v2, PEAT_INTEGER, "Argument error in second parameter of function [SetPPortState(<pport>,<integer>)]: ");   

    char x = (int)v2;
    mypport->SetDataState(x);


    return Variant(true);
}

Variant ConvertByteToList(int x)
{
    int d = 8;
    //	Variant buffer="";

    PList *list = new PList();
	for (;d>0;d--)
		{
            list->PushBack(Variant((int)(x & 1)));
            //buffer = buffer + Variant("|") + Variant((int)(x & 1));
            x >>= 1;

		}

    //buffer = buffer + Variant("|");

    counted_ptr<PEBLObjectBase> tmplist = counted_ptr<PEBLObjectBase>(list);
    PComplexData * tmpPCD= (new PComplexData(tmplist));
    Variant tmp = Variant(tmpPCD);
    delete tmpPCD;
    tmpPCD=NULL;
    return tmp;

}

Variant PrintByte(int x)
{
    int d = 8;
	Variant buffer="";

	for (;d>0;d--)
		{
            buffer = buffer + Variant("|") + Variant((int)(x & 1));
            x >>= 1;

		}

    buffer = buffer + Variant("|");

  return buffer;
}

// This gets the state of the parallel port.  It may be platform-specific.
//
Variant PEBLStream::GetPPortState(Variant v)
{
    //v[1] should have an pport 

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_PARALLELPORT, "Argument error in first parameter of function [GetPPortState(<pport>]: ");

    counted_ptr<PEBLObjectBase> tmp1 = (v1.GetComplexData())->GetObject();
    PParallelPort * mypport = dynamic_cast<PParallelPort*>(tmp1.get());
    

    char x1 = mypport->GetDataState();

    Variant bytelist = ConvertByteToList(x1);

    return bytelist;
}



// This sets the state of the parallel port to either input or output.
//
Variant PEBLStream::SetPPortMode(Variant v)
{
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_PARALLELPORT, "Argument error in first parameter of function [SetPPortMode(<pport>,<mode>)]: ");   

    counted_ptr<PEBLObjectBase> tmp1 = (v1.GetComplexData())->GetObject();
    PParallelPort * mypport = dynamic_cast<PParallelPort*>(tmp1.get());
    

    Variant v2 = plist->First();
    PError::AssertType(v2, PEAT_STRING, "Argument error in second parameter of function [SetPPortState(<pport>,<mode>)]: ");   

    if(v2 == "<input>")
        mypport->SetInputMode();
    else if(v2 == "<output>")
        mypport->SetOutputMode();
    else
        PError::SignalFatalError(Variant("Unknown mode type [")+v2+
                                 Variant("] in SetPPortState.  Must be <input> or <output>."));

    return Variant(true);
}

#if 0
// This waits for a change in the PPORT state (on the data bits)
//
Variant PEBLStream::WaitForPPortData(Variant v)
{
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_PARALLELPORT, "Argument error in first parameter of function [WaitForPPortData(<pport>)]: ");   

    counted_ptr<PEBLObjectBase> tmp1 = (v1.GetComplexData())->GetObject();
    PParallelPort * mypport = dynamic_cast<PParallelPort*>(tmp1.get());
    
    //Create a pport test correspending to keydown. 
    //1 is the value (down), DT_EQUAL is the test, key is the interface (e.g., the 'A' key) 
    PDevice * device = new PParallelPort(mypport);
    ValueState  * state = new ValueState(1, DT_EQUAL, key, device, PDT_KEYBOARD);

    //NULL,NULL will terminate the looping
    string  funcname = "";
    PList* params = NULL;
    Evaluator::mEventLoop.RegisterState(state,funcname, params);
    PEvent returnval = Evaluator::mEventLoop.Loop();

    //Now, clear the event loop tests
    Evaluator::mEventLoop.Clear();

    return Variant(returnval.GetDummyEvent().value);

}


// This waits for a change in the PPORT state (on the status bits)
//
Variant PEBLStream::WaitForPPortStatus(Variant v)
{
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_PARALLELPORT, "Argument error in first parameter of function [SetPPortMode(<pport>,<mode>)]: ");   

    counted_ptr<PEBLObjectBase> tmp1 = (v1.GetComplexData())->GetObject();
    PParallelPort * mypport = dynamic_cast<PParallelPort*>(tmp1.get());
    

    Variant v2 = plist->First();
    PError::AssertType(v2, PEAT_STRING, "Argument error in second parameter of function [SetPPortState(<pport>,<mode>)]: ");   

    if(v2 == "<input>")
        mypport->SetInputMode();
    else if(v2 == "<output>")
        mypport->SetOutputMode();
    else
        PError::SignalFatalError(Variant("Unknown mode type [")+v2+
                                 Variant("] in SetPPortState.  Must be <input> or <output>."));

    return Variant(true);
}
#endif


// This sets the state of the parallel port to either input or output.
//
Variant PEBLStream::OpenComPort(Variant v)
{

    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_INTEGER, "Argument error in first parameter of function [OpenComPort(<portnumber>,<baud>)]: ");



    Variant v2 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_INTEGER, "Argument error in first parameter of function [OpenComPort(<portnumber>),<baud>]: ");


    counted_ptr<PEBLObjectBase> tmp2 = counted_ptr<PEBLObjectBase>(new PComPort((int)v1,(int)v2));
    PComPort * cport = dynamic_cast<PComPort*>(tmp2.get());
    
    
    cport->Init();
            

    PComplexData * pcd = new PComplexData(tmp2);
    Variant tmp3 = Variant(pcd);
    delete pcd;
    pcd=NULL;
    return tmp3;
}



// 
//
Variant PEBLStream::ComPortGetByte(Variant v)
{
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_COMPORT, "Argument error in first parameter of function [ComPortGetByte(<port>)]: ");   

    counted_ptr<PEBLObjectBase> tmp1 = (v1.GetComplexData())->GetObject();
    PComPort * cport = dynamic_cast<PComPort*>(tmp1.get());
    
    unsigned char out;
    int retval= cport->GetByte(out);
    if(retval==0)
        return Variant(-1);
    else
        return Variant((int)out);
}

// 
//
Variant PEBLStream::ComPortSendByte(Variant v)
{
    PList * plist = v.GetComplexData()->GetList();
    Variant v1 = plist->First(); plist->PopFront();
    PError::AssertType(v1, PEAT_COMPORT, "Argument error in first parameter of function [ComPortSendByte(<port>,<int>)]: ");   

    Variant v2 = plist->First();
    //Second argumentcould be a bypte or a character.
    //    PError::AssertType(v2,PEAT_INTEGER,"Argument error in second parameter of function [ComPortSendByte(<port>,<int>)]: ");
        

    counted_ptr<PEBLObjectBase> tmp1 = (v1.GetComplexData())->GetObject();
    PComPort * cport = dynamic_cast<PComPort*>(tmp1.get());

    unsigned char send = 0;
    if(v2.IsInteger())
        {
            send = v2.GetInteger();    
        } else if(v2.IsString())
        {

            std::string tmp = (v2.GetString());
            send = tmp[0];
        }
    
    cport->PSendByte(send);
    
    return Variant(0);
}


