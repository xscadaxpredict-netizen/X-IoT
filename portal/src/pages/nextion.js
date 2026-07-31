import { icons } from '../utils/icons.js';
import { numberField, selectField, sectionCard } from '../components/form.js';

export const id = 'nextion';
export const title = 'HMI Display Configuration';
export const description = 'Configure the Nextion serial HMI display driver parameters.';

const baudRates = [
  { value: '2400', text: '2400' }, { value: '4800', text: '4800' },
  { value: '9600', text: '9600' }, { value: '19200', text: '19200' },
  { value: '38400', text: '38400' }, { value: '57600', text: '57600' },
  { value: '115200', text: '115200' },
];

export function render() {
  return sectionCard(icons.gear, 'Serial Interface', `
    <div class="field-grid">
      ${numberField({ id: 'nextion-rxPin', label: 'RX GPIO Pin', placeholder: '1', required: true })}
      ${numberField({ id: 'nextion-txPin', label: 'TX GPIO Pin', placeholder: '2', required: true })}
      ${selectField({ id: 'nextion-baudrate', label: 'Baud Rate', options: baudRates, required: true })}
      ${numberField({ id: 'nextion-updateIntervalMs', label: 'Update Interval (ms)', placeholder: '2000', required: true })}
    </div>
  `);
}

export function init() {}

export function setData(config) {
  if (!config.nextion) return;
  const n = config.nextion;
  const el = (id) => document.getElementById(id);
  if (n.rxPin) el('nextion-rxPin').value = n.rxPin;
  if (n.txPin) el('nextion-txPin').value = n.txPin;
  if (n.baudrate) el('nextion-baudrate').value = n.baudrate;
  if (n.updateIntervalMs) el('nextion-updateIntervalMs').value = n.updateIntervalMs;
}

export function getData() {
  const el = (id) => document.getElementById(id);
  return {
    nextion: {
      rxPin: parseInt(el('nextion-rxPin')?.value) || 1,
      txPin: parseInt(el('nextion-txPin')?.value) || 2,
      baudrate: parseInt(el('nextion-baudrate')?.value) || 9600,
      updateIntervalMs: parseInt(el('nextion-updateIntervalMs')?.value) || 2000,
    }
  };
}
