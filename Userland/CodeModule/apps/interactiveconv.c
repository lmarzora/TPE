//#include <lib.h>

void recibe_int();
void escribe_int();


int interactiveconv_main(){
	char buff[100];
	printLn("");
	printLn("----- Conversacion Interactiva -------");

	newMsgQueue("interactivconv", 1, sizeof(buff));

	newProcess("recibe_int", &recibe_int, 0, 0, 0);
	
	char * ans;
	ans = getLn(ans);

	putMsgQueue(ans, "interactivconv");

	ans = getLn();
	putMsgQueue(ans, "interactivconv");

	ans = getLn();
	putMsgQueue(ans, "interactivconv");
	
}

void recibe_int(){
	char buff[100];
	printLn("Bienvenido!");
	printLn("Como te llamas?");
	print("- ");
	
	getMsgQueue(buff, "interactivconv");
	
	printLn("-----------------");
	print("Hola ");
	printLn(buff);
	printLn("Como estas?");
	print("- ");
	
	getMsgQueue(buff, "interactivconv");

	printLn("-----------------");
	print(buff);
	printLn("?");
	printLn("Hay que estar GENIAL");
	printLn("Asi que va devuelta, como estas?");
	print("- ");

	getMsgQueue(buff, "interactivconv");

	printLn("-----------------");
	if(cmpstr(buff, "GENIAL") || cmpstr(buff, "genial")){
		printLn("AHORA SIII");
	}else{
		printLn("Booooo!!!");
	}

	printLn("");
	print("$ ");
	
}
