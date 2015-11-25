void consumir();
void producir();

int prodcons_main(){

	printLn("");
	printLn("Creando semaforos\n");
	newSemaphore("prod", 1);
	newSemaphore("cons", 0);
	
	printLn("Creando productor\n");
	newProcess("productor", &producir, 0, 0);

	printLn("Creando consumidor\n");
	newProcess("consumidor", &consumir, 0, 0);


}

void consumir(){
	int x = 0;
	while(x<10){
		waitSemaphore("prod");
		printLn("Consumo");
		signalSemaphore("cons");
		x++;
	}
	
}

void producir(){
	int y = 0;
	while(y<10){
		waitSemaphore("cons");
		printLn("Produzco");
		signalSemaphore("prod");
		y++;
	}
	
}