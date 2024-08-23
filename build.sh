gcc -o server main.c utilities/tolower.c serversettings/serversettings.c http/http.c webserverengine/webserverengine.c

valgrind --leak-check=yes ./server path "../vagdragonswebsite" port 8080
