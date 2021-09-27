./name ` docker-compose ps`
docker build -t pp .
docker-compose up -d
docker run -it --network dock_back-tier pp
