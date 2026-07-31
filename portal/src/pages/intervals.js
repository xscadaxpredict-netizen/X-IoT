import { icons } from '../utils/icons.js';
import { numberField, sectionCard } from '../components/form.js';

export const id = 'intervals';
export const title = 'Acquisition & Publish Intervals';
export const description = 'Configure the timing intervals for Modbus data acquisition and MQTT telemetry publishing.';

export function render() {
  return sectionCard(icons.clock, 'Timing Configuration', `
    <div class="field-grid">
      ${numberField({ id: 'acquisition-scanIntervalMs', label: 'Modbus Scan Interval (ms)', placeholder: '10000', required: true })}
      ${numberField({ id: 'publisher-publishIntervalMs', label: 'MQTT Publish Interval (ms)', placeholder: '20000', required: true })}
    </div>
  `);
}

export function init() {}

export function setData(config) {
  const el = (id) => document.getElementById(id);
  if (config.acquisition?.scanIntervalMs) el('acquisition-scanIntervalMs').value = config.acquisition.scanIntervalMs;
  if (config.publisher?.publishIntervalMs) el('publisher-publishIntervalMs').value = config.publisher.publishIntervalMs;
}

export function getData() {
  const el = (id) => document.getElementById(id);
  return {
    acquisition: { scanIntervalMs: parseInt(el('acquisition-scanIntervalMs')?.value) || 10000 },
    publisher: { publishIntervalMs: parseInt(el('publisher-publishIntervalMs')?.value) || 20000 },
  };
}
