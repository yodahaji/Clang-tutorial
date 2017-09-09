/***   CItutorial3.cpp   *****************************************
 * This code is licensed under the New BSD license.
 * See LICENSE.txt for details.
 * 
 * The CI tutorials remake the original tutorials but using the
 * CompilerInstance object which has as one of its purpose to 
 * create commonly  used Clang types.
 ****************************************************************/
#include <iostream>

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
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Frontend/Utils.h"
/*******************************************************************
 *This tutorial shows how to build a lexer valid with header files.
 ******************************************************************/
int main()
{
    using clang::CompilerInstance;
    using clang::TargetOptions;
    using clang::TargetInfo;
    using clang::FileEntry;
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
    TargetInfo *pTgtInfo = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pTgtOpt);
    ci.setTarget(pTgtInfo);

    // <Warning!!> -- Platform Specific Code lives here
    // This depends on A) that you're running linux and
    // B) that you have the same GCC LIBs installed that
    // I do. 
    // Search through Clang itself for something like this,
    // go on, you won't find it. The reason why is Clang
    // has its own versions of std* which are installed under 
    // /usr/local/lib/clang/<version>/include/
    // See somewhere around Driver.cpp:77 to see Clang adding
    // its version of the headers to its include path.   

    //Angled means Paths for '\#include <>' added by '-I'.
    //more detail can be found in clang::frontend::IncludeDirGroup
    ci.getHeaderSearchOpts().AddPath("/usr/include",
                                     clang::frontend::Angled,
                                     false,
                                     false);
    
    ci.getHeaderSearchOpts().AddPath("/usr/lib/gcc/x86_64-linux-gnu/5/include",
                                     clang::frontend::Angled,
                                     false,
                                     false);
    // </Warning!!> -- End of Platform Specific Code

    //create Preprocessor
    //put this stmt under the AddPaths is a Very Vital point!!!
    ci.createPreprocessor(clang::TU_Complete);

    
    //initializ the preprocessor
    //an important step to include the header file
    clang::InitializePreprocessor(ci.getPreprocessor(), 
                                  ci.getPreprocessorOpts(),
                                  ci.getPCHContainerReader(),
                                  ci.getFrontendOpts());

    //get the file pointer
    //"testInclude.c" could be any valid cpp filename
    const FileEntry *pFile = ci.getFileManager().getFile("testInclude.c");
    //Set the file ID for the main source file.
    ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID(
                                            pFile,
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
