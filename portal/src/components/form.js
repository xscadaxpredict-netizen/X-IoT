import { icons } from '../utils/icons.js';

export function textField({ id, label, placeholder = '', required = false }) {
  return `
    <div class="field">
      <label for="${id}">${label}${required ? ' <span class="req">*</span>' : ''}</label>
      <input type="text" id="${id}" placeholder="${placeholder}"${required ? ' data-required' : ''}>
      <div class="error-msg">${label} is required</div>
    </div>
  `;
}

export function numberField({ id, label, placeholder = '', required = false }) {
  return `
    <div class="field">
      <label for="${id}">${label}${required ? ' <span class="req">*</span>' : ''}</label>
      <input type="number" id="${id}" placeholder="${placeholder}"${required ? ' data-required' : ''}>
      <div class="error-msg">${label} is required</div>
    </div>
  `;
}

export function passwordField({ id, label, placeholder = '', required = false }) {
  return `
    <div class="field">
      <label for="${id}">${label}${required ? ' <span class="req">*</span>' : ''}</label>
      <div class="pw-wrap">
        <input type="password" id="${id}" placeholder="${placeholder}"${required ? ' data-required' : ''}>
        <button type="button" class="pw-toggle" title="Toggle visibility">${icons.eyeOpen}</button>
      </div>
      <div class="error-msg">${label} is required</div>
    </div>
  `;
}

export function selectField({ id, label, options = [], required = false }) {
  const optHtml = options.map(o => `<option value="${o.value}">${o.text}</option>`).join('');
  return `
    <div class="field">
      <label for="${id}">${label}${required ? ' <span class="req">*</span>' : ''}</label>
      <select id="${id}"${required ? ' data-required' : ''}>
        <option value="">Select ${label.toLowerCase()}</option>
        ${optHtml}
      </select>
      <div class="error-msg">${label} is required</div>
    </div>
  `;
}

export function toggleRow({ id, name, description, checked = false }) {
  return `
    <div class="option-row">
      <div class="option-info">
        <div class="option-name">${name}</div>
        <div class="option-desc">${description}</div>
      </div>
      <label class="switch"><input type="checkbox" id="${id}"${checked ? ' checked' : ''}><span class="track"></span></label>
    </div>
  `;
}

export function sectionCard(icon, title, bodyHTML) {
  return `
    <div class="section">
      <div class="section-header">
        ${icon}
        ${title}
      </div>
      <div class="section-body">
        ${bodyHTML}
      </div>
    </div>
  `;
}
