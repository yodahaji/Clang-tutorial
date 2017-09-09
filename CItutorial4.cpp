/***   CItutorial4.cpp   *******************************************
 * This code is licensed under the New BSD license.
 * See LICENSE.txt for details.
 * 
 * The CI tutorials remake the original tutorials but using the
 * CompilerInstance object which has as one of its purpose to 
 * create commonly used Clang types.
 *******************************************************************/
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
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"

/****************************************************************
 *This tutorial shows how to use the ParseAST,the key class
 *to produce AST.
 **************************************************************/
int main()
{
    using clang::CompilerInstance;
    using clang::TargetOptions;
    using clang::TargetInfo;
    using clang::FileEntry;
    using clang::Token;
    /***********************************************************
     *ASTConsumer - This is an abstract interface that should be 
     *implemented by clients that read ASTs. 
     *This abstraction layer allows the client to be independent
     *of the AST producer (e.g. parser vs AST dump file reader,etc). 
     ***********************************************************/
    using clang::ASTConsumer;

    //create compiler instance and complex initialization
    CompilerInstance ci;
    ci.createDiagnostics();
    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());

    //set the target
    std::shared_ptr<clang::TargetOptions> pTgtOpt = 
        std::make_shared<clang::TargetOptions>();
    pTgtOpt->Triple = 
        llvm::sys::getDefaultTargetTriple();
    TargetInfo *pTgrInfo = TargetInfo::CreateTargetInfo(
                                ci.getDiagnostics(), 
                                pTgtOpt);
    ci.setTarget(pTgrInfo);

    //create Preprocessor
    ci.createPreprocessor(clang::TU_Complete);

    //set ASTConsumer to prepare the AST reading.
    ci.setASTConsumer(llvm::make_unique<ASTConsumer>());

    //create ASTContext,which owns all information about the AST
    ci.createASTContext();
    //Sema - This implements semantic analysis and AST 
    //building for C.
    //
    //second argument means CodeCompletionConsumer,
    //the Abstract interface for a consumer of 
    //code-completion information. 
    ci.createSema(clang::TU_Complete, NULL);

    //get the file pointer
    const FileEntry *pFile = 
        ci.getFileManager().getFile("test.c");
    //Set the file ID for the main source file.
    ci.getSourceManager().setMainFileID(
        ci.getSourceManager().createFileID(
            pFile, clang::SourceLocation(), 
            clang::SrcMgr::C_User));
    
    //Use ParseAST,second argument is true,
    //it will dump some Statics about the AST.
    //third argument is true when we dont need
    //the AST information about the function body.
    clang::ParseAST(ci.getSema(),true,false);

    return 0;
}
