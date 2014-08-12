<html><body> {{! This page has no head section.}}
{{#USER_INFO}}
Welcome back <a href="/login">{{USERNAME}}</a><br>
{{/USER_INFO}}

List example:<ol>
{{#LIST_EXAMPLE}}
<li> {{LIST_KEY}} := {{LIST_VALUE}} </li>
{{/LIST_EXAMPLE}}
</ol>

{{FOOTER}}
</body></html>
