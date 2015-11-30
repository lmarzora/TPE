int max;

void a_proc();
void b_proc();

int abab_main(){
	max = 30;
	printLn("");
	printLn("Creando procesos A");
	newProcess("a_print", &a_proc, 0, 0, 0);
	printLn("Creando procesos B");
	newProcess("b_print", &b_proc, 0, 0, 0);

	while(max>0);

	printLn("\n");
	print("$ ");
	return 0;
}

void a_proc(){
	sleep(1);
	while(0<max){
		print("a");
		yield();
		max--;
	}
	
}

void b_proc(){
	while(0<max){
		print("b");
		yield();
		max--;
	}
	
}