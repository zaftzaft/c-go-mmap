package main

import (
	"encoding/binary"
	"fmt"
	"os"
	"syscall"
	"time"
	"unsafe"
)

type Hoge struct {
	num  int
	num2 int
}

func Run() int {
	f, err := os.OpenFile("/dev/shm/c-go-mmap", os.O_RDONLY, 0664)

	if err != nil {
		fmt.Println(err)
		return 1
	}
	defer f.Close()

	hoge := Hoge{}

	data, err := syscall.Mmap(int(f.Fd()), 0, int(unsafe.Sizeof(hoge)), syscall.PROT_READ, syscall.MAP_SHARED)
	if err != nil {
		fmt.Println(err)
		return 1
	}

	for {
		hoge.num = int(binary.LittleEndian.Uint32(data[0:4]))
		fmt.Println(hoge.num)
		time.Sleep(1 * time.Second)
	}

	return 0
}

func main() {
	os.Exit(Run())
}
