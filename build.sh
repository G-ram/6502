LIB_TARGET=6502.a
make -C lib
ln -s lib/headers test/src/headers
ln -s lib/lib6502.a test/src/$LIB_TARGET
make -C test/src
ln -s lib/headers nes/lib/headers
ln -s lib/lib6502.a nes/lib/$LIB_TARGET
make -C nes/lib
#DOESN'WORK
export GOPATH=`pwd`
#go build -o ../../../nes
