Feature('Basic');

Scenario('test something', ({ I }) => {
    I.amOnPage('/rest');
    I.see('Specified request cannot be processed.');
});
