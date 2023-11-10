import sys
from pydbus import SystemBus
from gi.repository import GLib
import dbus.service

BLUEZ_SERVICE_NAME = 'org.bluez'
GATT_MANAGER_IFACE = 'org.bluez.GattManager1'
ADAPTER_IFACE = 'org.bluez.Adapter1'
DEVICE_IFACE = 'org.bluez.Device1'

# 블루투스 어댑터 이름을 변경하는 함수
def set_local_adapter_name(bus, adapter_path, name):
    adapter = bus.get(BLUEZ_SERVICE_NAME, adapter_path)
    adapter.Alias = name
    print(f"Adapter name set to '{name}'")

class PinCodeCharacteristic(dbus.service.Object):
    GATT_CHRC_IFACE = 'org.bluez.GattCharacteristic1'
    NOTIFICATION_IFACE = 'org.freedesktop.DBus.Properties'

    def __init__(self, bus, index, service_path):
        self.value = []
        self.path = f'{service_path}/char{index}'
        self.bus = bus
        super().__init__(bus, self.path)
    
    @dbus.service.method(GATT_CHRC_IFACE, in_signature='a{sv}', out_signature='')
    def StartNotify(self, options):
        pass

    @dbus.service.method(GATT_CHRC_IFACE, in_signature='a{sv}', out_signature='')
    def StopNotify(self):
        pass

    @dbus.service.method(GATT_CHRC_IFACE, in_signature='aya{sv}', out_signature='ay')
    def WriteValue(self, value, options):
        print(f'Received pin code: {"".join(map(chr, value))}')
        self.value = value
        if self.value == bytearray(b'1234'):
            print("Pin code is correct! Exiting with success status.")
            sys.exit(0)  # 성공 상태로 프로그램 종료
        else:
            print("Pin code is incorrect. Exiting with error status.")
            sys.exit(1)  # 실패 상태로 프로그램 종료

# 시스템 버스에 연결
bus = SystemBus()

# 각종 매니저와 어댑터의 경로를 찾습니다.
adapter_path = '/org/bluez/hci0'
service_path = f'{adapter_path}/service0'

# 블루투스 어댑터 이름을 'KETIIPE'로 설정합니다.
set_local_adapter_name(bus, adapter_path, 'KETIIPE')

# 서비스와 캐릭터리스틱을 등록합니다.
pin_code_characteristic = PinCodeCharacteristic(bus, 0, service_path)

# GLib 이벤트 루프를 시작합니다.
mainloop = GLib.MainLoop()
try:
    mainloop.run()
except KeyboardInterrupt:
    mainloop.quit()
except SystemExit as e:
    # GLib 이벤트 루프가 종료될 때 종료 코드를 설정합니다.
    exit_code = e.code
    mainloop.quit()
    sys.exit(exit_code)
