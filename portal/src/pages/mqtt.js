import { icons } from '../utils/icons.js';
import { textField, passwordField, numberField, toggleRow, sectionCard } from '../components/form.js';

export const id = 'mqtt';
export const title = 'MQTT Broker Configuration';
export const description = 'Configure the MQTT v3.1.1 broker connection and session parameters.';

export function render() {
  return `
    ${sectionCard(icons.globe, 'Broker Endpoint', `
      <div class="field-grid">
        ${textField({ id: 'mqtt-broker', label: 'Broker Address', placeholder: 'mqtt.example.com', required: true })}
        ${numberField({ id: 'mqtt-port', label: 'Port', placeholder: '1883', required: true })}
      </div>
    `)}
    ${sectionCard(icons.key, 'Authentication', `
      <div class="field-grid">
        ${textField({ id: 'mqtt-username', label: 'Username', placeholder: 'Broker username', required: true })}
        ${passwordField({ id: 'mqtt-password', label: 'Password', placeholder: 'Broker password', required: true })}
        ${textField({ id: 'mqtt-clientId', label: 'Client Identifier', placeholder: 'XP-GATEWAY-01', required: true })}
        ${numberField({ id: 'mqtt-keepAlive', label: 'Keep Alive (seconds)', placeholder: '120', required: true })}
      </div>
      <div class="options-group">
        ${toggleRow({ id: 'mqtt-secure', name: 'TLS / SSL', description: 'Encrypt transport with TLS' })}
        ${toggleRow({ id: 'mqtt-cleanSession', name: 'Clean Session', description: 'Discard pending state on connect' })}
        ${toggleRow({ id: 'mqtt-autoReconnect', name: 'Auto Reconnect', description: 'Re-establish connection on broker disconnect', checked: true })}
      </div>
    `)}
  `;
}

export function init() {}

export function setData(config) {
  if (!config.mqtt) return;
  const m = config.mqtt;
  const el = (id) => document.getElementById(id);
  if (m.broker) el('mqtt-broker').value = m.broker;
  if (m.port) el('mqtt-port').value = m.port;
  if (m.username) el('mqtt-username').value = m.username;
  if (m.password) el('mqtt-password').value = m.password;
  if (m.clientId) el('mqtt-clientId').value = m.clientId;
  if (m.keepAlive) el('mqtt-keepAlive').value = m.keepAlive;
  el('mqtt-secure').checked = m.secure === true;
  el('mqtt-cleanSession').checked = m.cleanSession === true;
  el('mqtt-autoReconnect').checked = m.autoReconnect !== false;
}

export function getData() {
  const el = (id) => document.getElementById(id);
  return {
    mqtt: {
      broker: el('mqtt-broker')?.value.trim() || '',
      port: parseInt(el('mqtt-port')?.value) || 1883,
      username: el('mqtt-username')?.value.trim() || '',
      password: el('mqtt-password')?.value.trim() || '',
      clientId: el('mqtt-clientId')?.value.trim() || '',
      keepAlive: parseInt(el('mqtt-keepAlive')?.value) || 120,
      secure: el('mqtt-secure')?.checked ?? false,
      cleanSession: el('mqtt-cleanSession')?.checked ?? false,
      autoReconnect: el('mqtt-autoReconnect')?.checked ?? true,
    }
  };
}
