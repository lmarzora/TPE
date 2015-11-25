void consumir();
void producir();

int prodcons_main(){

	printLn("");
	printLn("Creando semaforos\n");
	newSemaphore("prod", 5);
	newSemaphore("cons", 0);
	
	printLn("");
	printLn("Creando productor\n");
	newProcess("productor", &producir, 0, 0);

	printLn("");
	printLn("Creando consumidor\n");
	newProcess("consumidor", &consumir, 0, 0);


}

void consumir(){
	while(1){
		waitSemaphore("prod");
		printLn("Consumo");
		signalSemaphore("cons");
	}
	
}

void producir(){
	while(1){
		waitSemaphore("cons");
		printLn("Produzco");
		signalSemaphore("prod");
	}
	
}