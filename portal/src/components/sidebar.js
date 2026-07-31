import { icons } from '../utils/icons.js';
import { navigate } from '../utils/router.js';

export function renderSidebar() {
  return `
    <div class="nav-section">
      <div class="nav-section-title">Overview</div>
      <button class="nav-item active" data-panel="dashboard">${icons.dashboard} Dashboard</button>
    </div>
    <div class="nav-section">
      <div class="nav-section-title">Network</div>
      <button class="nav-item" data-panel="wifi">${icons.wifi} Wi-Fi</button>
      <button class="nav-item" data-panel="mqtt">${icons.cloud} MQTT Broker</button>
    </div>
    <div class="nav-section">
      <div class="nav-section-title">Fieldbus</div>
      <button class="nav-item" data-panel="modbus">${icons.terminal} Modbus RTU</button>
      <button class="nav-item" data-panel="tags">${icons.tag} Tag Registry</button>
    </div>
    <div class="nav-section">
      <div class="nav-section-title">System</div>
      <button class="nav-item" data-panel="intervals">${icons.clock} Intervals</button>
      <button class="nav-item" data-panel="nextion">${icons.monitor} HMI Display</button>
      <button class="nav-item" data-panel="security">${icons.lock} Security</button>
    </div>
    <div class="nav-section">
      <div class="nav-section-title">Maintenance</div>
      <button class="nav-item" data-panel="ota">${icons.download} Firmware Update</button>
    </div>
    <div class="sidebar-footer">
      <div class="fw-info">Firmware v2.0.0<br>Xpredict Automation Pvt Ltd</div>
    </div>
  `;
}

export function initSidebar() {
  document.querySelectorAll('.nav-item').forEach(item => {
    item.addEventListener('click', () => navigate(item.dataset.panel));
  });
}
