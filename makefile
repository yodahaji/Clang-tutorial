CLANG_LIBS=-lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewrite -lclangRewriteFrontend -lclangEdit -lclangAST -lclangLex -lclangBasic -lclang

prog=CItutorial1\
	 CItutorial2\
	 CItutorial3\
	 CItutorial4\
	 CIturorial6\
	 CIBasicRecursiveASTVisitor\
	 CIrewriter

flag1=`llvm-config --cxxflags --ldflags`

flag2=`llvm-config --libs --system-libs`

cc=g++

all:CItutorial1 CItutorial2 CItutorial3 CItutorial4 CItutorial6 CIrewriter CIBasicRecursiveASTVisitor
	$(CItutorial1) $(CItutorial2) $(CItutorial3) $(CItutorial4) $(CItutorial6) $(CIrewriter) $(CIBasicRecursiveASTVisitor)

CItutorial1:CItutorial1.cpp
	$(cc) $(flag1) CItutorial1.cpp $(CLANG_LIBS) $(flag2) -o CItutorial1

CItutorial2:CItutorial2.cpp
	$(cc) $(flag1) CItutorial2.cpp $(CLANG_LIBS) $(flag2) -o CItutorial2

CItutorial3:CItutorial3.cpp
	$(cc) $(flag1) CItutorial3.cpp $(CLANG_LIBS) $(flag2) -o CItutorial3

CItutorial4:CItutorial4.cpp
	$(cc) $(flag1) CItutorial4.cpp $(CLANG_LIBS) $(flag2) -o CItutorial4

CItutorial6:CItutorial6.cpp
	$(cc) $(flag1) CItutorial6.cpp $(CLANG_LIBS) $(flag2) -o CItutorial6

CIrewriter:CIrewriter.cpp
	$(cc) $(flag1) CIrewriter.cpp $(CLANG_LIBS) $(flag2) -o CIrewriter

CIBasicRecursiveASTVisitor:CIBasicRecursiveASTVisitor.cpp
	$(cc) $(flag1) CIBasicRecursiveASTVisitor.cpp $(CLANG_LIBS) $(flag2) -o CIBasicRecursiveASTVisitor

