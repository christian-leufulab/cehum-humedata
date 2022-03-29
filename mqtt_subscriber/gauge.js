let dashboardData = null;

let gaugeOptions = {
    credits: {
        enabled: false
    },
    chart: {
        type: 'solidgauge'
    },

    title: null,

    pane: {
        center: ['50%', '85%'],
        size: '140%',
        startAngle: -90,
        endAngle: 90,
        background: {
            backgroundColor:
                Highcharts.defaultOptions.legend.backgroundColor || '#EEE',
            innerRadius: '60%',
            outerRadius: '100%',
            shape: 'arc'
        }
    },

    exporting: {
        enabled: false
    },

    tooltip: {
        enabled: false
    },

    // the value axis
    yAxis: {
        stops: [
            [0.1, '#DF5353'], // green
            [0.5, '#DDDF0D'], // yellow
            [0.9, '#55BF3B'] // red
        ],
        lineWidth: 0,
        tickWidth: 0,
        minorTickInterval: null,
        tickAmount: 2,
        title: {
            y: -70
        },
        labels: {
            y: 16
        }
    },

    plotOptions: {
        solidgauge: {
            dataLabels: {
                y: 5,
                borderWidth: 0,
                useHTML: true
            }
        }
    }
};

let chartAtmPressure = Highcharts.chart('container-atm-pressure', Highcharts.merge(gaugeOptions, {
    yAxis: {
        min: 0,
        max: 200,
        title: {
            text: 'Presión Atmosférica'
        }
    },

    credits: {
        enabled: false
    },

    series: [{
        name: 'Presión Atmosférica',
        data: [atm_pres],
        dataLabels: {
            format:
                '<div style="text-align:center">' +
                '<span style="font-size:25px">{y}</span><br/>' +
                '<span style="font-size:12px;opacity:0.4">kPa</span>' +
                '</div>'
        },
        tooltip: {
            valueSuffix: ' kPa'
        }
    }]

}));

// The RPM gauge
let chartRpm = Highcharts.chart('container-rpm', Highcharts.merge(gaugeOptions, {
    yAxis: {
        min: 0,
        max: 5,
        title: {
            text: 'Presión Interna'
        }
    },

    series: [{
        name: 'Presión Interna',
        data: [int_pres],
        dataLabels: {
            format:
                '<div style="text-align:center">' +
                '<span style="font-size:25px">{y:.1f}</span><br/>' +
                '<span style="font-size:12px;opacity:0.4">' +
                'kPa' +
                '</span>' +
                '</div>'
        },
        tooltip: {
            valueSuffix: ' kPa'
        }
    }]

}));

// Bring life to the dials

setInterval(function () {


    let req = new XMLHttpRequest();
    req.open('GET', '/humedata/update_dashboard.php', true);
    req.onreadystatechange = function (aEvt) {
        if (req.readyState == 4) {
            if (req.status == 200) {
                // console.log(req.responseText);
                // console.log(req);
                // console.log(JSON.parse(req.response));
                dashboardData = JSON.parse(req.response);
                point = chartAtmPressure.series[0].points[0];
                point.update(dashboardData.atm_pressure);
            }
            else
                console.info("Error loading page\n");
        }
    };
    req.send(null);

//     const url = "/humedata/update_dashboard.php";
//     fetch(url)
//   .then(
//     response => response.text() // .json(), .blob(), etc.
//   ).then(
//     text => console.log(text) // Handle here
//   );


    // Speed
    // let point,
    //     newVal,
    //     inc;

    // if (chartSpeed) {
    //     point = chartSpeed.series[0].points[0];
    //     inc = Math.round((Math.random() - 0.5) * 100);
    //     newVal = point.y + inc;

    //     if (newVal < 0 || newVal > 200) {
    //         newVal = point.y - inc;
    //     }

    //     point.update(newVal);
    // }

    // // RPM
    // if (chartRpm) {
    //     point = chartRpm.series[0].points[0];
    //     inc = Math.random() - 0.5;
    //     newVal = point.y + inc;

    //     if (newVal < 0 || newVal > 5) {
    //         newVal = point.y - inc;
    //     }

    //     point.update(newVal);
    // }
// }, 120000);
}, 2000);
