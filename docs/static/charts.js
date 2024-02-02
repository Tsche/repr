
function render_chart(target_id, chart) {
  $.getJSON(chart, function(data) {
     return new frappe.Chart(target_id, data);
  });
}
