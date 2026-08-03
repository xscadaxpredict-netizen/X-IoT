import { icons } from '../utils/icons.js';
import { getSystemInfo } from '../api/system.js';

export function renderTopbar() {
  return `
    <div class="topbar-brand">
      <button class="mobile-menu-btn" id="menu-toggle">${icons.menu}</button>
      ${icons.cpu}
      <span class="brand-name">XPREDICT GATEWAY</span>
      <span class="brand-sub">Configuration Portal</span>
    </div>
    <div class="topbar-status">
      <div class="status-indicator" id="topbar-ip" style="font-size: 12px; color: var(--text-muted); display: flex; align-items: center; gap: 6px;">
        ${icons.globe} <span id="topbar-ip-text">Loading...</span>
      </div>
    </div>
  `;
}

export function initTopbar() {
  fetchIP();
  setInterval(fetchIP, 10000); // Refresh every 10 seconds
}

async function fetchIP() {
  try {
    const info = await getSystemInfo();
    const el = document.getElementById('topbar-ip-text');
    if (el && info.ipAddress) {
      el.textContent = info.ipAddress;
      el.style.color = (info.ipAddress !== '0.0.0.0' && info.ipAddress !== 'Acquiring IP...') 
        ? 'var(--success)' 
        : 'var(--warning)';
    }
  } catch (e) {
    const el = document.getElementById('topbar-ip-text');
    if (el) {
      el.textContent = 'Offline';
      el.style.color = 'var(--error)';
    }
  }
}