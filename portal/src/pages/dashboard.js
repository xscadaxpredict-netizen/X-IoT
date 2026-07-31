import { icons } from '../utils/icons.js';
import { getSystemInfo } from '../api/system.js';

export const id = 'dashboard';
export const title = 'System Dashboard';
export const description = 'Real-time system health and resource utilization.';
export const showActions = false;

export function render() {
  return `
    <div class="stats-grid">
      <div class="stat-card">
        <div class="stat-icon">${icons.activity}</div>
        <div class="stat-info"><div class="stat-label">Uptime</div><div class="stat-value" id="sys-uptime">--</div></div>
      </div>
      <div class="stat-card">
        <div class="stat-icon">${icons.memory}</div>
        <div class="stat-info"><div class="stat-label">Free Heap</div><div class="stat-value" id="sys-heap">--</div></div>
      </div>
      <div class="stat-card">
        <div class="stat-icon">${icons.cpu}</div>
        <div class="stat-info"><div class="stat-label">CPU Frequency</div><div class="stat-value" id="sys-cpu">--</div></div>
      </div>
      <div class="stat-card">
        <div class="stat-icon">${icons.wifi}</div>
        <div class="stat-info"><div class="stat-label">Wi-Fi RSSI</div><div class="stat-value" id="sys-rssi">--</div></div>
      </div>
    </div>
  `;
}

export function init() { fetchSystemInfo(); }

async function fetchSystemInfo() {
  try {
    const info = await getSystemInfo();
    if (info.uptime) document.getElementById('sys-uptime').textContent = formatUptime(info.uptime);
    if (info.freeHeap) document.getElementById('sys-heap').textContent = `${(info.freeHeap / 1024).toFixed(1)} KB`;
    if (info.cpuFreqMHz) document.getElementById('sys-cpu').textContent = `${info.cpuFreqMHz} MHz`;
    if (info.rssi !== undefined) document.getElementById('sys-rssi').textContent = `${info.rssi} dBm`;
  } catch (e) { /* endpoint not available yet */ }
}

function formatUptime(seconds) {
  const d = Math.floor(seconds / 86400);
  const h = Math.floor((seconds % 86400) / 3600);
  const m = Math.floor((seconds % 3600) / 60);
  if (d > 0) return `${d}d ${h}h ${m}m`;
  if (h > 0) return `${h}h ${m}m`;
  return `${m}m`;
}

export function setData() {}
export function getData() { return {}; }
