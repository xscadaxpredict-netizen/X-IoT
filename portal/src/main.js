import './styles/variables.css';
import './styles/base.css';
import './styles/layout.css';
import './styles/components.css';

import { renderTopbar, initTopbar } from './components/topbar.js';
import { renderSidebar, initSidebar } from './components/sidebar.js';
import { showToast } from './components/toast.js';
import { registerPage, navigate, setConfigData } from './utils/router.js';
import { validateRequired } from './utils/validation.js';
import { icons } from './utils/icons.js';
import { apiGet, apiPost } from './api/client.js';

import * as dashboardPage from './pages/dashboard.js';
import * as wifiPage from './pages/wifi.js';
import * as mqttPage from './pages/mqtt.js';
import * as modbusPage from './pages/modbus.js';
import * as tagsPage from './pages/tags.js';
import * as intervalsPage from './pages/intervals.js';
import * as nextionPage from './pages/nextion.js';
import * as securityPage from './pages/security.js';

const allPages = [dashboardPage, wifiPage, mqttPage, modbusPage, tagsPage, intervalsPage, nextionPage, securityPage];

function init() {
  document.getElementById('topbar').innerHTML = renderTopbar();
  document.getElementById('sidebar').innerHTML = renderSidebar();
  initSidebar();
  initTopbar();
  allPages.forEach(p => registerPage(p.id, p));

  // Render ALL panels (hidden by default, shown by router)
  const content = document.getElementById('page-content');
  let html = '';
  allPages.forEach((p, i) => {
    html += `<div class="panel${i === 0 ? ' active' : ''}" id="panel-${p.id}">
      <div class="page-header"><h2>${p.title}</h2><p>${p.description}</p></div>
      ${p.render()}
    </div>`;
  });
  content.innerHTML = html;

  // Initialize all pages
  allPages.forEach(p => { if (p.init) p.init(); });

  // Set action bar button icons
  document.getElementById('btn-save').innerHTML = `${icons.check} Save Configuration`;
  document.getElementById('btn-reboot').innerHTML = `${icons.refresh} Reboot Gateway`;

  // Mobile menu
  document.getElementById('menu-toggle')?.addEventListener('click', () => {
    document.getElementById('sidebar').classList.toggle('open');
  });

  // Delegated password toggle
  document.addEventListener('click', (e) => {
    const toggle = e.target.closest('.pw-toggle');
    if (!toggle) return;
    const input = toggle.closest('.pw-wrap').querySelector('input');
    if (input.type === 'password') { input.type = 'text'; toggle.innerHTML = icons.eyeOff; }
    else { input.type = 'password'; toggle.innerHTML = icons.eyeOpen; }
  });

  // Delegated validation clear
  document.addEventListener('input', (e) => {
    if (e.target.hasAttribute('data-required')) e.target.classList.remove('error');
  });
  document.addEventListener('change', (e) => {
    if (e.target.hasAttribute('data-required')) e.target.classList.remove('error');
  });

  // Save
  document.getElementById('btn-save').addEventListener('click', handleSave);
  // Reboot
  document.getElementById('btn-reboot').addEventListener('click', handleReboot);

  // Default page
  navigate('dashboard');
  // Load config
  loadConfig();
}

async function loadConfig() {
  try {
    const config = await apiGet('/api/config');
    setConfigData(config);
    allPages.forEach(p => { if (p.setData) p.setData(config); });
  } catch (e) {
    showToast('Failed to load device configuration', 'error');
  }
}

async function handleSave() {
  const { valid, firstInvalidField } = validateRequired();
  if (!valid) {
    if (firstInvalidField) {
      const panel = firstInvalidField.closest('.panel');
      if (panel) navigate(panel.id.replace('panel-', ''));
    }
    showToast('Please complete all required fields', 'warning');
    return;
  }
  const config = {};
  allPages.forEach(p => { if (p.getData) Object.assign(config, p.getData()); });

  const btnSave = document.getElementById('btn-save');
  const btnReboot = document.getElementById('btn-reboot');
  btnSave.disabled = true; btnReboot.disabled = true;
  btnSave.innerHTML = `<svg fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2" style="animation:spin 1s linear infinite"><path stroke-linecap="round" stroke-linejoin="round" d="M4 4v5h.582m15.356 2A8.001 8.001 0 004.582 9m0 0H9m11 11v-5h-.581m0 0a8.003 8.003 0 01-15.357-2m15.357 2H15"/></svg> Saving...`;

  try {
    const res = await apiPost('/api/config', config);
    if (res.status === 'ok') {
      showToast('Configuration saved \u2014 gateway rebooting', 'success');
      btnSave.innerHTML = 'Rebooting...';
      setTimeout(() => window.location.reload(), 4000);
    } else {
      showToast(res.message || 'Save failed', 'error');
      resetSaveBtn();
    }
  } catch (e) {
    showToast('Connection error', 'error');
    resetSaveBtn();
  }
}

function resetSaveBtn() {
  const btnSave = document.getElementById('btn-save');
  const btnReboot = document.getElementById('btn-reboot');
  btnSave.disabled = false; btnReboot.disabled = false;
  btnSave.innerHTML = `${icons.check} Save Configuration`;
}

async function handleReboot() {
  if (!confirm('Reboot the gateway? Active connections will be dropped.')) return;
  const btnSave = document.getElementById('btn-save');
  const btnReboot = document.getElementById('btn-reboot');
  btnSave.disabled = true; btnReboot.disabled = true;
  btnReboot.textContent = 'Rebooting...';
  try { await apiPost('/api/reboot'); } catch (e) {}
  showToast('Reboot command dispatched', 'success');
  setTimeout(() => window.location.reload(), 4000);
}

document.addEventListener('DOMContentLoaded', init);
