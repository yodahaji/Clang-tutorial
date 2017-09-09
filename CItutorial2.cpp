/***   CItutorial2.cpp   *************************************************
 * This code is licensed under the New BSD license.
 * See LICENSE.txt for details.
 * 
 * The CI tutorials remake the original tutorials but using the
 * CompilerInstance object which has as one of its purpose to create 
 * commonly used Clang types.
 ***********************************************************************/
#include<iostream>

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/Diagnostic.h"

/************************************************************************
 *This tutorial shows how to build a Lexer to dump tokens a c/c++file
 *without header files.
 ***********************************************************************/
int main()
{
    using clang::CompilerInstance;
    using clang::TargetOptions;
    using clang::TargetInfo;
    /**************************************************
     *FileEntry class store some information about
     *a file in the disk or in the virtual file system.
     *e.g.,the name,the size,etc.
     **************************************************/
    using clang::FileEntry;
    /*************************************************
     *This structure provides full information about 
     *a lexed token. 
     *For example,theTokenKind,location,length and 
     *so on.
     ***********************************************/
    using clang::Token;

    //create compiler instance and complex initialization
    CompilerInstance ci;
    ci.createDiagnostics();
    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());

    //set the target
    std::shared_ptr<clang::TargetOptions> pTgtOpt = 
        std::make_shared<clang::TargetOptions>();
    pTgtOpt->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *pTgtInfo = 
        TargetInfo::CreateTargetInfo(ci.getDiagnostics(), 
                                    pTgtOpt);
    ci.setTarget(pTgtInfo);

    //create Preprocessor
    //it should be the next to last step to initialiazation.
    ci.createPreprocessor(clang::TU_Complete);

    //get the file pointer
    //"hello.cc" could be any valid cpp filename  without include path
    const FileEntry *pFile = ci.getFileManager().getFile("hello.cc");
    //Set the file ID for the main source file.
    ci.getSourceManager().setMainFileID(ci.getSourceManager().createFileID(pFile,
                                                    clang::SourceLocation(),
                                                    clang::SrcMgr::C_User));
    //Enter the specified FileID as the main source file.
    ci.getPreprocessor().EnterMainSourceFile();
    //Marked the start of processing a single input.
    ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                             &ci.getPreprocessor());
    Token tok;
    do {
        //lexer the file.
        ci.getPreprocessor().Lex(tok);
        //error handling.
        if( ci.getDiagnostics().hasErrorOccurred())
            break;
        //print the token.
        ci.getPreprocessor().DumpToken(tok);
        std::cerr << std::endl;
    } while ( tok.isNot(clang::tok::eof));

    //Marked the end of processing of a single input.
    ci.getDiagnosticClient().EndSourceFile();

    return 0;
}
