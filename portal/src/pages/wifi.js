import { icons } from '../utils/icons.js';
import { textField, passwordField, numberField, toggleRow, sectionCard } from '../components/form.js';

export const id = 'wifi';
export const title = 'Wi-Fi Station Configuration';
export const description = 'Configure the wireless network credentials for upstream connectivity.';

export function render() {
  return sectionCard(icons.gear, 'Connection Parameters', `
    <div class="field-grid">
      ${textField({ id: 'wifi-ssid', label: 'Network SSID', placeholder: 'Enter network name', required: true })}
      ${passwordField({ id: 'wifi-password', label: 'WPA2 Password', placeholder: 'Enter network key', required: true })}
      ${numberField({ id: 'wifi-reconnectTimeoutMs', label: 'Reconnect Timeout (ms)', placeholder: '20000', required: true })}
    </div>
    <div class="options-group">
      ${toggleRow({ id: 'wifi-autoReconnect', name: 'Auto Reconnect', description: 'Automatically reconnect when the link drops', checked: true })}
    </div>
  `);
}

export function init() {}

export function setData(config) {
  if (!config.wifi) return;
  const w = config.wifi;
  const el = (id) => document.getElementById(id);
  if (w.ssid) el('wifi-ssid').value = w.ssid;
  if (w.password) el('wifi-password').value = w.password;
  if (w.reconnectTimeoutMs) el('wifi-reconnectTimeoutMs').value = w.reconnectTimeoutMs;
  el('wifi-autoReconnect').checked = w.autoReconnect !== false;
}

export function getData() {
  const el = (id) => document.getElementById(id);
  return {
    wifi: {
      ssid: el('wifi-ssid')?.value.trim() || '',
      password: el('wifi-password')?.value.trim() || '',
      autoReconnect: el('wifi-autoReconnect')?.checked ?? true,
      reconnectTimeoutMs: parseInt(el('wifi-reconnectTimeoutMs')?.value) || 20000
    }
  };
}
