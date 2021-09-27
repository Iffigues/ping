package main

import (
	"os"
	"bytes"
	"log"
	"fmt"
	"os/exec"
)

func strncmp(a,b string, o int) (i int) {
	bsize := len(b)
	asize := len(a)
	if bsize < o  || asize < o {
		return 0;
	}
	for p := 0; p < asize; p++ {
		if (a[p] != b[p]) {
			return 0;
		}
	}
	return 1
}

func getmd(a string)(tt string) {
	cmd := exec.Command("docker", "inspect", "-f", "'{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}'", a)
	var out bytes.Buffer
	cmd.Stdout = &out
	err := cmd.Run()
	if err != nil {
		log.Fatal(err)
	}
	return out.String()
}

func main() {
	j := len("dock_ping")
	tt := os.Args[6:]
	f, err := os.OpenFile("network.txt", os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0755)

	if err != nil {
	}
	defer f.Close()
	for _,val := range tt {
		if strncmp("dock_ping",val, j) == 1 {
			fmt.Println(val)
			_, err = fmt.Fprintf(f, "%s: %s" ,val, getmd(val))
		}

	}
}
