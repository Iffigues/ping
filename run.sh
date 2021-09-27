mkdir app
mv main.go app
mv ps.txt app
cd app
go mod init pp
go build
./pp `cat ps.txt`
sh
