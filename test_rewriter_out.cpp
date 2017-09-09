#define L_AND(a, b) a && b
#define L_OR(a, b) a || b

// Begin function main returning integer
int main() {
	bool a = true;
	bool b = false;

	if( L_OR((L_AND(a , b)) , ( L_AND(b , a)))) {
	}
}
// End function main

