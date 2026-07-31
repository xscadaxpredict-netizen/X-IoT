import { icons } from '../utils/icons.js';

export function renderTopbar() {
  return `
    <div class="topbar-brand">
      <button class="mobile-menu-btn" id="menu-toggle">${icons.menu}</button>
      ${icons.cpu}
      <span class="brand-name">XPREDICT GATEWAY</span>
      <span class="brand-sub">Configuration Portal</span>
    </div>
    <div class="topbar-status">
      <div class="status-indicator">
        <span class="status-dot"></span>
        System Online
      </div>
    </div>
  `;
}
