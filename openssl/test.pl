#!/usr/bin/perl
use strict;
use warnings;

my @algs = qw(kyber512 ledakem128sln02 newhope512cca ntrukem443 ntrulpr761 sikep503);

# Single part tests
foreach my $alg (@algs) {
    run_test($alg);
}

# Two part hybrid : classical + QS
foreach my $alg (@algs) {
    run_test("X25519,".$alg);
}

# Two part hybrid : QS + QS
foreach my $alg1 (@algs) {
    foreach my $alg2 (@algs) {
        next if $alg1 eq $alg2;
        run_test($alg1.",".$alg2);
    }
}

# Three part hybrid : classic + QS + QS
foreach my $alg1 (@algs) {
    foreach my $alg2 (@algs) {
        next if $alg1 eq $alg2;
        run_test("X25519,".$alg1.",".$alg2);
    }
}

sub run_test {
    my ($groups) = @_;

    print "Testing groups $groups ...";

    # Spawn s_server
    my $execcmd = "openssl s_server -accept localhost:0 -engine nistpqc -groups $groups -cert server.pem -key server.key";
    my $server_fh;
    my $server_pid = open($server_fh, "$execcmd 2>&1 |") or die "Failed to $execcmd: $!\n";
    my $server_port = 0;

    # Process the output from s_server until we find the ACCEPT line, which
    # tells us what the accepting address and port are.
    while (<$server_fh>) { 
        s/\R$//;                # Better chomp
        next unless (/^ACCEPT\s.*:(\d+)$/);
        $server_port = $1;
        last;
    }

    # If there was no ACCEPT line then s_server must have exited
    if ($server_port == 0) {
        waitpid($server_pid, 0);
        die "no ACCEPT detected in '$execcmd' output: $?\n";
    }

    # Spawn s_client
    $execcmd = "openssl s_client -connect localhost:$server_port -state -engine nistpqc -groups $groups -CAfile root_ca.pem -no_ssl3 -no_tls1 -no_tls1_1 -no_tls1_2";
    my $client_fh;
    my $client_pid = open($client_fh, "$execcmd 2>&1 |") or die "Failed to $execcmd: $!\n";

    # Scan client output looking for the magic line
    my $success = 0;
    my $client_log = "";
    while (<$client_fh>) {
        $client_log = $client_log.$_;
        s/\R$//;                # Better chomp
        next unless (/^SSL_connect:SSL negotiation finished successfully$/);
        $success = 1;
        last;
    }

    # Kill s_server and s_client
    kill 9, $client_pid;
    kill 9, $server_pid;

    if ($success) {
        print "pass\n";
    } else {
        print "fail\n";
        print "Client output:\n$client_log";
        die;
    }
}
