# Shared Secret with Stenography

### Compiling the files
Inside the `stenography-shared-secret` folder run:
```bash
make 
```

### Distributing an image
Let's say we want to distribute the image `Albert.bmp` to all the shadow images in `files/` and setting a `k` of 5:
```bash
./ss d /path/to/Albert.bmp 5 /path/to/files/   
```

### Recovering an image
Let's say we want to recover the secret image from the shadow images in `files/` with a `k` of 5:
```bash
./ss r /path/to/save/secret.bmp 5 /path/to/files/   
```