"""Compile production C++ with controlled HAL/HTTP, then validate its JSON output."""
from pathlib import Path
import json
import os
import subprocess
import tempfile

here = Path(__file__).resolve().parent
library = Path(os.environ.get('ARDUINO_MANAGER_LIBRARY', str(Path.home() / 'Arduino/libraries/ArduinoManager')))
sources = ['network/esp8266/ESP8266ComponentService.cpp',
           'network/esp8266/ESP8266WifiResourceClient.cpp',
           'components/DHT11Component.cpp', 'components/RGBLedComponent.cpp', 'utils/RGB.cpp', 'core/Component.cpp',
           'core/Pin.cpp', 'core/DigitalPin.cpp', 'core/AnalogicPin.cpp', 'utils/List.cpp']
with tempfile.TemporaryDirectory(prefix='arduino-component-test-') as output:
    binary = str(Path(output) / 'components')
    subprocess.run(['g++', '-std=c++11', '-DESP8266', '-Wall', '-Wextra',
                    '-I', str(here / 'native'), '-I', str(library / 'src'),
                    str(here / 'native/components.cpp'),
                    *[str(library / 'src' / s) for s in sources], '-o', binary], check=True)
    result = subprocess.run([binary], text=True, capture_output=True)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
records = {name: json.loads(body) for name, body in
           (line.split('\t', 1) for line in result.stdout.splitlines())}
contract = json.loads((here / 'fixtures/dht11-telemetry.json').read_text())
assert records['initial']['components'][0] == contract['components'][0]
assert records['retry']['components'][0] == records['initial']['components'][0]
assert records['quick_reconnect'] == records['reconnect']
for label, body in records.items():
    for component in body['components']:
        expected = {'climate': ['temperature', 'humidity'],
                    'climate2': ['temperature2', 'humidity2'], 'led': ['led']}[component['attr']]
        assert list(component) == ['attr', 'values']
        assert [v['attr'] for v in component['values']] == expected
        for value in component['values']:
            assert set(value) == {'attr', 'value', 'healthy', 'updatePolicy', 'expectedIntervalSeconds'}
            periodic = component['attr'] == 'climate'
            assert value['updatePolicy'] == ('PERIODIC' if periodic else 'ON_CHANGE')
            assert value['expectedIntervalSeconds'] == (10 if periodic else None)
            assert value['healthy'] == (value['value'] is not None)
        if len(component['values']) == 2:
            assert len({v['healthy'] for v in component['values']}) == 1
for label in ['initial', 'status', 'retry', 'failure', 'failure_status', 'failure_retry', 'reconnect']:
    assert 'climate' in [s['attr'] for s in records[label]['components']]
for label in ['initial', 'status', 'retry', 'reconnect']:
    assert len(records[label]['components']) == 3
assert records['initial']['components'][0]['values'][0]['value'] == 24
assert records['initial']['components'][0]['values'][1]['value'] == 60
assert records['periodic']['components'][0]['values'][0]['value'] == 27
for label in ['failure', 'failure_status', 'failure_retry', 'initial_failure', 'malformed']:
    assert all(v['value'] is None and not v['healthy'] for v in records[label]['components'][0]['values'])
assert records['reconnect']['components'][0]['values'][0]['value'] == 28
assert records['reconnect']['components'][1]['values'][1]['value'] == 65
print('PASS: real C++ driver/service/client; HAL, HTTP, grouped JSON, retry, status, writes, reconnect')
