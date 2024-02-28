# Weather Station
Its simple weather station project, use
* main processor: **Raspberry Pi W Pico**,
* DHT 11, 
* Barometer,
* three color ePaper display (1.54").

Check current weather forecast im preset localization, used OpenWeatherMap API to get local data.

# How to build
## Linux:
```bash
$ cd src/
$ mkdir build && cd $_
$ cmake .. && make
$ cp /media/$USER/RP ... main.uf2
```

# Todo:
- [x] Measure humidity,
- [x] Measure temperature,
- [x] Measure pressure,
- [ ] display in graph on ePaper collected data,
- [ ] connected to OWM API,
