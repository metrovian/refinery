(() => {
  function sanitizeHexInput(value) {
    return value
      .toUpperCase()
      .replace(/[^0-9A-F\s,;:-]/g, "")
      .replace(/[,:;-]+/g, " ")
      .replace(/\s+/g, " ")
      .trimStart();
  }

  function hasHexInput(value) {
    return value.trim().length > 0;
  }

  window.hexInputUtils = {
    sanitizeHexInput,
    hasHexInput,
  };
})();
