Feature('Integration Admin');

Scenario('Bad password', async ({ I }) => {
    var resp = await I.sendPostRequest('/V1/integration/admin/token',
        {username:"admin", password:"123123q"});

    I.assertEqual(resp.status,403);
    I.assertEqual(resp.data.code,403);
    I.assertEqual(resp.data.message,"Access forbidden");
});


Scenario('Get Admin Token', async ({ I }) => {
    var resp = await I.sendPostRequest('/V1/integration/admin/token',
        {username:"admin", password:"123123qa"});

    I.assertEqual(resp.status,200);
});
