#Check if "libssl-dev" is installed

#Server
g++ -o ssl-server SSL-Server.cpp -lssl -lcrypto
sudo ./ssl-server <port>

#Client
g++ -o ssl-client SSL-Client.cpp -lssl -lcrypto
./ssl-client <ip/localhost> <port>
