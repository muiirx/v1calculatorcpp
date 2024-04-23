const { exec } = require('child_process');

new Vue({
    el: '#app',
    data: {
        input: '',
        result: '',
        debuggindk: '',
        logg: ''
    },
    methods: {
        calculate() {
            exec(`./calculator "${this.input}"`, (error, stdout, stderr) => {
                if (error) {
                    console.error(`exec error: ${error}`);

                    return;
                }
                if (stderr) {
                    console.error(`stderr: ${stderr}`);
                    return;
                }
                this.result = stdout.trim();
            });
        },
        debugg() {
            exec(`./calculator "${this.debuggindk}"`, (error, stdout, stderr) => {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }
                if (stderr) {
                    console.error(`stderr: ${stderr}`);
                    return;
                }
                this.logg = stdout.trim();
        });
        }
    }
});
