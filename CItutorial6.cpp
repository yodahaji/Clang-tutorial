/***   CItutorial6.cpp   *****************************************************
 * This code is licensed under the New BSD license.
 * See LICENSE.txt for details.
 * 
 * The CI tutorials remake the original tutorials but using the
 * CompilerInstance object which has as one of its purpose to create commonly
 * used Clang types.
 *****************************************************************************/
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
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"
/******************************************************************************
 *This tutorial shows another way to use AST.So it could be  regarded as
 *the same version with tutorial4 appended with header files.
 *****************************************************************************/
class MyASTConsumer : public clang::ASTConsumer
{
public:
    MyASTConsumer() : clang::ASTConsumer() { }
    virtual ~MyASTConsumer() { }

    /*HandleTopLevelDecl - Handle the specified top-level declaration.
     *This is called by the parser to process every top-level Decl*.
     ******************************************************************/
    virtual bool HandleTopLevelDecl( clang::DeclGroupRef d)
    {
        static int count = 0;
        clang::DeclGroupRef::iterator it;
        for( it = d.begin(); it != d.end(); it++)
        {
            count++;
            clang::VarDecl *vd = llvm::dyn_cast<clang::VarDecl>(*it);
            if(!vd)
            {
                continue;
            }
            if( vd->isFileVarDecl() && !vd->hasExternalStorage() )
            {
                std::cerr << "Read top-level variable decl: '";
                std::cerr << vd->getDeclName().getAsString() ;
                std::cerr << std::endl;
            }
        }
        return true;
    }
};


int main()
{
    using namespace clang;

    //create compiler instance and complex initialization
    CompilerInstance ci;
    ci.createDiagnostics();
    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());

    //set the target
    std::shared_ptr<clang::TargetOptions> pto = std::make_shared<clang::TargetOptions>();
    pto->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pto);
    ci.setTarget(pti);

    //set LanguageOptions
    ci.getLangOpts().CPlusPlus=1;
    
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
    ci.createPreprocessor(clang::TU_Complete);

    //create AST environment
    ci.setASTConsumer(llvm::make_unique<MyASTConsumer>());
    ci.createASTContext();

    //get the file pointer
    const FileEntry *pFile = ci.getFileManager().getFile("input04.c");
    //Set the file ID for the main source file.
    ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_User));

    //Start to process the file
    ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                             &ci.getPreprocessor());
    //parse the ast                                        
    clang::ParseAST(ci.getPreprocessor(), &ci.getASTConsumer(), ci.getASTContext());

    //end processing the file
    ci.getDiagnosticClient().EndSourceFile();

    return 0;
}
