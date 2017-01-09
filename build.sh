cd lib
make lib
cd go
export GOPATH=`pwd`
cd src
go build -o ../../nes
cd ..
