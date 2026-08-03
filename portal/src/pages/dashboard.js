import { icons } from '../utils/icons.js';
import { getSystemInfo } from '../api/system.js';

export const id = 'dashboard';
export const title = 'System Dashboard';
export const description = 'Real-time system health and resource utilization.';
export const showActions = false;

let refreshTimer = null;

export function render() {
  return `
    <!-- ── System Overview ── -->
    <div class="dashboard-section">
      <div class="dashboard-section-header">
        <div class="dashboard-section-icon">${icons.activity}</div>
        <div class="dashboard-section-title">System Overview</div>
      </div>
      <div class="kpi-grid">
        <div class="kpi-card">
          <div class="kpi-label">Firmware</div>
          <div class="kpi-value sm" id="kpi-fw">--</div>
        </div>
        <div class="kpi-card">
          <div class="kpi-label">Uptime</div>
          <div class="kpi-value" id="kpi-uptime">--</div>
        </div>
        <div class="kpi-card">
          <div class="kpi-label">Free Heap (RAM)</div>
          <div class="kpi-value" id="kpi-heap">--</div>
        </div>
        <div class="kpi-card">
          <div class="kpi-label">Min Free Heap</div>
          <div class="kpi-value" id="kpi-min-heap">--</div>
        </div>
        <div class="kpi-card">
          <div class="kpi-label">CPU Temperature</div>
          <div class="kpi-value" id="kpi-temp">--</div>
        </div>
      </div>
    </div>

    <!-- ── Flash Storage ── -->
    <div class="dashboard-section">
      <div class="dashboard-section-header">
        <div class="dashboard-section-icon">${icons.hardDrive}</div>
        <div class="dashboard-section-title">Flash Storage (LittleFS)</div>
      </div>
      <div class="kpi-grid">
        <div class="kpi-card">
          <div class="kpi-label">Usage</div>
          <div class="kpi-value sm" id="kpi-flash-text">--</div>
          <div class="kpi-bar-track">
            <div class="kpi-bar-fill" id="kpi-flash-bar" style="width: 0%"></div>
          </div>
        </div>
        <div class="kpi-card">
          <div class="kpi-label">Total Capacity</div>
          <div class="kpi-value" id="kpi-flash-total">--</div>
        </div>
      </div>
    </div>

    <!-- ── Wi-Fi ── -->
    <div class="dashboard-section">
      <div class="dashboard-section-header">
        <div class="dashboard-section-icon">${icons.wifi}</div>
        <div class="dashboard-section-title">Wi-Fi</div>
      </div>
      <div class="kpi-grid">
        <div class="kpi-card">
          <div class="kpi-label">Status</div>
          <div class="kpi-value sm" id="kpi-wifi-status">--</div>
        </div>
        <div class="kpi-card">
          <div class="kpi-label">IP Address</div>
          <div class="kpi-value sm" id="kpi-wifi-ip">--</div>
        </div>
        <div class="kpi-card">
          <div class="kpi-label">Signal (RSSI)</div>
          <div class="kpi-value" id="kpi-wifi-rssi">--</div>
        </div>
      </div>
    </div>

    <!-- ── MQTT Broker ── -->
    <div class="dashboard-section">
      <div class="dashboard-section-header">
        <div class="dashboard-section-icon">${icons.cloud}</div>
        <div class="dashboard-section-title">MQTT Broker</div>
      </div>
      <div class="kpi-grid">
        <div class="kpi-card">
          <div class="kpi-label">Connection</div>
          <div class="kpi-value sm" id="kpi-mqtt-status">--</div>
        </div>
      </div>
    </div>

    <!-- ── Fieldbus ── -->
    <div class="dashboard-section">
      <div class="dashboard-section-header">
        <div class="dashboard-section-icon">${icons.terminal}</div>
        <div class="dashboard-section-title">Fieldbus</div>
      </div>
      <div class="kpi-grid">
        <div class="kpi-card">
          <div class="kpi-label">Active Tags</div>
          <div class="kpi-value" id="kpi-tags">--</div>
        </div>
        <div class="kpi-card">
          <div class="kpi-label">Publisher</div>
          <div class="kpi-value sm" id="kpi-publisher">--</div>
        </div>
      </div>
    </div>
  `;
}

export function init() {
  fetchSystemInfo();
  refreshTimer = setInterval(fetchSystemInfo, 5000);
}

async function fetchSystemInfo() {
  try {
    const info = await getSystemInfo();

    // ── System Overview ──
    const fwEl = document.getElementById('kpi-fw');
    if (fwEl && info.fwVersion) fwEl.textContent = `v${info.fwVersion}`;

    if (info.uptime !== undefined) {
      document.getElementById('kpi-uptime').textContent = formatUptime(info.uptime);
    }
    if (info.freeHeap !== undefined) {
      document.getElementById('kpi-heap').textContent = `${(info.freeHeap / 1024).toFixed(1)} KB`;
    }
    if (info.minFreeHeap !== undefined) {
      const minEl = document.getElementById('kpi-min-heap');
      const minKB = (info.minFreeHeap / 1024).toFixed(1);
      minEl.textContent = `${minKB} KB`;
      // Warn if min heap drops below 20KB
      minEl.style.color = info.minFreeHeap < 20480 ? 'var(--error)' : 'var(--text-primary)';
    }
    if (info.cpuTemp !== undefined) {
      document.getElementById('kpi-temp').textContent = `${info.cpuTemp.toFixed(1)} °C`;
    }

    // ── Flash Storage ──
    if (info.flashTotal && info.flashUsed !== undefined) {
      const totalKB = (info.flashTotal / 1024).toFixed(0);
      const usedKB = (info.flashUsed / 1024).toFixed(0);
      const percent = Math.round((info.flashUsed / info.flashTotal) * 100);
      document.getElementById('kpi-flash-text').textContent = `${usedKB} / ${totalKB} KB (${percent}%)`;
      document.getElementById('kpi-flash-total').textContent = `${totalKB} KB`;
      document.getElementById('kpi-flash-bar').style.width = `${percent}%`;
      document.getElementById('kpi-flash-bar').style.background =
        percent > 85 ? 'var(--error)' : 'var(--accent)';
    }

    // ── Wi-Fi ──
    const wifiStatusEl = document.getElementById('kpi-wifi-status');
    const wifiIpEl = document.getElementById('kpi-wifi-ip');
    const wifiRssiEl = document.getElementById('kpi-wifi-rssi');

    if (info.wifiStateCode !== undefined) {
      const state = info.wifiStateCode;
      if (state === 154) {
        wifiStatusEl.innerHTML = '<span class="status-dot online"></span>Connected';
      } else if (state === 152) {
        wifiStatusEl.innerHTML = '<span class="status-dot waiting"></span>Acquiring IP...';
      } else if (state === 151) {
        wifiStatusEl.innerHTML = '<span class="status-dot waiting"></span>Connecting...';
      } else if (state === 153) {
        wifiStatusEl.innerHTML = '<span class="status-dot offline"></span>Retrying...';
      } else {
        wifiStatusEl.innerHTML = '<span class="status-dot offline"></span>Disconnected';
      }
    }

    wifiIpEl.textContent = info.ipAddress || '0.0.0.0';

    if (info.rssi !== undefined) {
      wifiRssiEl.textContent = `${info.rssi} dBm`;
    } else {
      wifiRssiEl.textContent = '--';
    }

    // ── MQTT ──
    const mqttEl = document.getElementById('kpi-mqtt-status');
    if (info.mqttStatus) {
      mqttEl.innerHTML = '<span class="status-dot online"></span>Online';
    } else {
      mqttEl.innerHTML = '<span class="status-dot offline"></span>Offline';
    }

    // ── Fieldbus ──
    if (info.activeTags !== undefined) {
      document.getElementById('kpi-tags').textContent = info.activeTags;
    }
    const pubEl = document.getElementById('kpi-publisher');
    if (info.publisherStatus) {
      pubEl.innerHTML = '<span class="status-dot online"></span>Running';
    } else {
      pubEl.innerHTML = '<span class="status-dot offline"></span>Stopped';
    }

  } catch (e) {
    console.error('Failed to fetch system info', e);
  }
}

function formatUptime(seconds) {
  const d = Math.floor(seconds / 86400);
  const h = Math.floor((seconds % 86400) / 3600);
  const m = Math.floor((seconds % 3600) / 60);
  const s = seconds % 60;
  if (d > 0) return `${d}d ${h}h ${m}m`;
  if (h > 0) return `${h}h ${m}m`;
  return `${m}m ${s}s`;
}

export function setData() {}
export function getData() { return {}; }