from pathlib import Path
import subprocess
import os
import tempfile
root=Path(os.environ.get('ARDUINO_DATA_DIR', str(Path.home()/'.arduino15'))) / 'packages/esp8266'
library = Path(os.environ.get('ARDUINO_MANAGER_LIBRARY', str(Path.home()/'Arduino/libraries/ArduinoManager')))
output = tempfile.TemporaryDirectory(prefix='arduino-wifi-check-')
core=root/'hardware/esp8266/3.1.2'
compiler=next((root/'tools/xtensa-lx106-elf-gcc').glob('*/bin/xtensa-lx106-elf-g++'))
includes=[core/'cores/esp8266',core/'variants/d1_mini',core/'tools/sdk/include',core/'tools/sdk/lwip2/include',core/'tools/sdk/libc/xtensa-lx106-elf/include',library/'src']
includes += list((core/'libraries').glob('*/src')) + list((core/'libraries').glob('*'))
flags=['-std=gnu++17','-c','-Os','-mlongcalls','-mtext-section-literals','-fno-rtti','-fno-exceptions','-DLED_BUILTIN=2','-DESP8266','-DARDUINO_ARCH_ESP8266','-DARDUINO=10819','-DF_CPU=80000000L','-DMMU_IRAM_SIZE=0x8000','-DMMU_ICACHE_SIZE=0x8000','-DLWIP_OPEN_SRC','-DTCP_MSS=536','-DVTABLES_IN_FLASH','-DLWIP_IPV6=0','-DLWIP_FEATURES=1','-D__ets__','-DICACHE_FLASH','-U__STRICT_ANSI__']
for inc in includes:flags += ['-I',str(inc)]
files=list((library/'src/network/esp8266').glob('*.cpp'))
files += list((library/'src/components').glob('*.cpp'))
files += list((library/'src/examples').glob('*.ino'))
for source in files:
 print(source.name,flush=True)
 subprocess.run([str(compiler),*flags,'-x','c++',str(source),'-o',str(Path(output.name)/(source.name+'.o'))],check=True)
