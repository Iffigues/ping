docker create --name my-nginx \
  --network my-net \
  --publish 8080:80 \
  nginx:latest
