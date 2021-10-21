// extern function
#ifdef _cplusplus
extern "C"
#endif
void foo();

int main(void) {
	foo(); // Should write to STDOUT "Hello World!\n"
	return 0;
}