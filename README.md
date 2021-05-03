# ebpf-call

*ebpf-call* is an example of an eBPF program and uBPF-based loader that allows calling external non-helper functions.
The loader registers a set of functions in exts.c, which the example(s) can invoke without recompiling the loader.

This functionality requires the uBPF VM provided by ITU.
https://github.com/ITU-DASYALab/ubpf/

## Usage

To run ebpf-call, first clone this repository.

```
git clone https://github.com/niclashedam/ebpf-call
```

Compile *ebpf-call* along with the example(s).

```
make all
```

You can now load the example(s).

```
./ebpf-call ./examples/basic.o
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
