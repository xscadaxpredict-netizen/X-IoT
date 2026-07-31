export function validateRequired() {
  clearErrors();
  let valid = true;
  let firstInvalidField = null;
  document.querySelectorAll('[data-required]').forEach(field => {
    if (field.value.trim() === '') {
      field.classList.add('error');
      valid = false;
      if (!firstInvalidField) firstInvalidField = field;
    }
  });
  return { valid, firstInvalidField };
}

export function clearErrors() {
  document.querySelectorAll('.error').forEach(el => el.classList.remove('error'));
}
