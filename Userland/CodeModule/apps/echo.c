

void echo_main(){

	char * line;
	int flag = 1;
 

	while(flag){
		line = getLn(line);
		if(!cmpstr(line, "")){
			if(cmpstr(line, "exit")){
				flag = 0;
			}else if(cmpstr(line, "ps")){
				listProcesses();
			}else if(cmpstr(line, "beyond stack")){
				beyondStack();
			}else{
				print("echo: ");
				print(line);
				printLn("");
				print("$ ");
			}
		}
	}
	printLn("");
	print("$ ");
}