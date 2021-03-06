# Shared Secret with Stenography

## Compiling the files
Inside the `stenography-shared-secret` folder run:
```bash
make 
```
This will create an executable file called `ss`.

## Executing program
File must be run using
```bash
./ss (d|r) secret-file.bmp k shadows-directory/
```
If there's an error when executing, an error message will be shown and the code will return `EXIT_FAILURE`. If successful, nothing is shown and the code will return `EXIT_SUCCESS`.

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