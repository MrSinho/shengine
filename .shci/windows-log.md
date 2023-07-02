
# shengine

![windows-badge](windows-exit_code.svg)

## [windows build logs:](https://github.com/mrsinho/shci)

        

Build ran for `0.17s`

---

```bash
choco install -y curl mingw cmake
mingw-get install gcc mingw32-make gdb

```

```bash

```

---

    
```bash
cd shengine
python export-application.py name=triangle target=EXECUTABLE path=applications/triangle
python export-application.py name=noise target=EXECUTABLE path=applications/noise
cd applications/triangle/windows/build
cmake --build .
cd ../../../noise/windows/build
cmake --build .

```

```bash

```

---

    

Build terminated with exit code 1

---

