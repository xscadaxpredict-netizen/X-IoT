import { icons } from '../utils/icons.js';

let toastTimer = null;

export function showToast(message, type = 'success') {
  const el = document.getElementById('toast');
  const iconMap = {
    success: icons.check,
    error: icons.alertCircle,
    warning: icons.alertTriangle
  };
  el.innerHTML = `${iconMap[type] || ''} ${message}`;
  el.className = `toast ${type} show`;
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => el.classList.remove('show'), 3500);
}
