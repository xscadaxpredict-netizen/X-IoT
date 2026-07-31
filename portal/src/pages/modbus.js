import { icons } from '../utils/icons.js';
import { numberField, selectField, sectionCard } from '../components/form.js';

export const id = 'modbus';
export const title = 'Modbus RTU Configuration';
export const description = 'Configure the RS-485 serial bus parameters for Modbus RTU communication.';

const baudRates = [
  { value: '2400', text: '2400' }, { value: '4800', text: '4800' },
  { value: '9600', text: '9600' }, { value: '19200', text: '19200' },
  { value: '38400', text: '38400' }, { value: '57600', text: '57600' },
  { value: '115200', text: '115200' },
];
const frameFormats = [
  { value: 'SERIAL_8N1', text: '8N1 \u2014 8 Data, No Parity, 1 Stop' },
  { value: 'SERIAL_8N2', text: '8N2 \u2014 8 Data, No Parity, 2 Stop' },
  { value: 'SERIAL_8E1', text: '8E1 \u2014 8 Data, Even Parity, 1 Stop' },
  { value: 'SERIAL_8O1', text: '8O1 \u2014 8 Data, Odd Parity, 1 Stop' },
];

export function render() {
  return sectionCard(icons.gear, 'Serial Interface', `
    <div class="field-grid">
      ${selectField({ id: 'modbus-baudrate', label: 'Baud Rate', options: baudRates, required: true })}
      ${selectField({ id: 'modbus-serialConfig', label: 'Frame Format', options: frameFormats, required: true })}
      ${numberField({ id: 'modbus-txPin', label: 'TX GPIO Pin', placeholder: '17', required: true })}
      ${numberField({ id: 'modbus-rxPin', label: 'RX GPIO Pin', placeholder: '18', required: true })}
      ${numberField({ id: 'modbus-timeoutMs', label: 'Response Timeout (ms)', placeholder: '2000', required: true })}
    </div>
  `);
}

export function init() {}

export function setData(config) {
  if (!config.modbus) return;
  const m = config.modbus;
  const el = (id) => document.getElementById(id);
  if (m.baudrate) el('modbus-baudrate').value = m.baudrate;
  if (m.serialConfig) el('modbus-serialConfig').value = m.serialConfig;
  if (m.txPin) el('modbus-txPin').value = m.txPin;
  if (m.rxPin) el('modbus-rxPin').value = m.rxPin;
  if (m.timeoutMs) el('modbus-timeoutMs').value = m.timeoutMs;
}

export function getData() {
  const el = (id) => document.getElementById(id);
  return {
    modbus: {
      baudrate: parseInt(el('modbus-baudrate')?.value) || 9600,
      serialConfig: el('modbus-serialConfig')?.value || 'SERIAL_8N1',
      txPin: parseInt(el('modbus-txPin')?.value) || 17,
      rxPin: parseInt(el('modbus-rxPin')?.value) || 18,
      timeoutMs: parseInt(el('modbus-timeoutMs')?.value) || 2000,
    }
  };
}
