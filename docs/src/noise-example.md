# Noise application example

[TOC]

![](media/noise-3.png)

## Build

Using Windows (run as admin):
```bash
python export-application.py name=noise target=EXECUTABLE path=applications/noise
cd applications/noise/build/windows
cmake --build .
cp ../../ini.smd ../../bin/windows/ini.smd
```

Using Linux:
```bash
sudo python3.9 export-application.py name=noise target=EXECUTABLE path=applications/noise
cd applications/noise/build/linux
sudo cmake --build .
cp ../../ini.smd ../../bin/linux/ini.smd
```

## Application tour

To change the values of the parameters `S` `A` and `B`:

|Parameter    |Increase key    |Decrease key    |
|-------------|----------------|----------------|
| `S`         |       _W_      |       _Q_      |
| `A`         |       _S_      |       _A_      |
| `B`         |       _X_      |       _Z_      |

To see the parameters values, or to enable/disable the UI, press `H`. 

Fragment shader algorithm:

\f$  c = \cos(\pi x y s)\f$  


\f$
RGB_{raw} = \begin{bmatrix}
\sin(\pi |x| {s \over a}) + c\\
\sin(\pi |y| {s \over b}) + c\\
\cos(\pi  s  {x \over y}) + c\\
\end{bmatrix}
\f$

You can export these values to `applications/noise/smd/noise-saved.smd` by pressing `LEFT_CTRL` + `E`. When the application is reloaded (`LEFT_CTRL` + `R`) the default parameters written at `applications/noise/smd/noise-recovery.smd` will be read. You can load the exported values by pressing `LEFT_CTRL` + `L`.

## Example parameters

\f$  s = {1 \over 2} \f$  

\f$  a = 1\f$  

\f$  b = -2,5\f$  

![](media/noise-1.png)



\f$  s = {1 \over 2} \f$  

\f$  a = 1\f$  

\f$  b = -2,5\f$  

![](media/noise-2.png)



\f$  s = 4\f$  

\f$  a = 4,5\f$  

\f$  b = -1,8\f$  

![](media/noise-3.png)



\f$  s = 4\f$  

\f$  a = 5\f$  

\f$  b = 7\f$  

![](media/noise-5.png)



