void recibe();
void escribe();


int simpleconv_main(){
	char buff[50];
	newMsgQueue("simpleconv", 1, sizeof(buff));

	newProcess("recibe", &recibe, 0, 0, 0);
	newProcess("escribe", &escribe, 0, 0, 0);
	
}

void recibe(){
	int j = 0;
	char buff[50];
	while(j<6){
		printLn("----- Espero mensaje -----");
		getMsgQueue(buff, "simpleconv");
		print("Recibi: ");
		printLn(buff);
		printLn("---------------------------");
		j++;
	}

	deleteMsgQueue("simpleconv");
	printLn("");
	print("$ ");
	
	
}

void escribe(){
	int i = 0;
	while(i<6){
		char * textos[] = {"Hola", "Don", "Pepito", "Hola", "Don", "Jose"};
		printLn("****** Escribo mensaje ******");
		print("Envio ");
		printLn(textos[i]);
		putMsgQueue(textos[i], "simpleconv");
		printLn("***************************");
		i++;
	}
	

}