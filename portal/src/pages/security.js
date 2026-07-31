import { icons } from '../utils/icons.js';
import { textField, passwordField, sectionCard } from '../components/form.js';

export const id = 'security';
export const title = 'Portal Security';
export const description = 'Configure the HTTP Basic Authentication credentials for this configuration portal.';

export function render() {
  return sectionCard(icons.key, 'Admin Credentials', `
    <div class="field-grid">
      ${textField({ id: 'portal-username', label: 'Username', placeholder: 'admin', required: true })}
      ${passwordField({ id: 'portal-password', label: 'Password', placeholder: 'Enter password', required: true })}
    </div>
  `);
}

export function init() {}

export function setData(config) {
  if (!config.portal) return;
  const p = config.portal;
  const el = (id) => document.getElementById(id);
  if (p.username) el('portal-username').value = p.username;
  if (p.password) el('portal-password').value = p.password;
}

export function getData() {
  const el = (id) => document.getElementById(id);
  return {
    portal: {
      username: el('portal-username')?.value.trim() || '',
      password: el('portal-password')?.value.trim() || '',
    }
  };
}
